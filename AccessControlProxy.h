/**
 * @file AccessControlProxy.h
 * @brief Declares the project interfaces and types.
 */

#ifndef ACCESSCONTROLPROXY_H
#define ACCESSCONTROLPROXY_H

#include "WorkItem.h"
#include "AccessPolicy.h"
#include "Participant.h"
#include "WorkflowData.h"
#include "include/WorkflowEvent.h"
#include "include/WorkflowEventPublisher.h"
#include "Role.h"


/**
 * @brief Represents the AccessControlProxy component of the workflow engine.
 */
class AccessControlProxy : public WorkItem {
public:
    /**
     * @brief Performs the AccessControlProxy operation.
     * @param real Parameter used by this operation.
     * @param policy Parameter used by this operation.
     * @param events Parameter used by this operation.
     */
    AccessControlProxy(WorkItem* real, const AccessPolicy* policy, WorkflowEventPublisher* events = nullptr)
        : WorkItem(real->getId(), real->getName()), real_(real), policy_(policy), events_(events) {}

    /**
     * @brief Performs the AccessControlProxy operation.
     */
    ~AccessControlProxy() override { delete real_; }

    /**
     * @brief Performs the assign operation.
     * @param p Parameter used by this operation.
     * @param actor Parameter used by this operation.
     */
    void assign(Participant& p, Participant& actor) override {
        if (check(actor, "assign")) real_->assign(p, actor);
    }
    /**
     * @brief Performs the acceptAssignment operation.
     * @param actor Parameter used by this operation.
     */
    void acceptAssignment(Participant& actor) override {
        if (check(actor, "acceptAssignment")) real_->acceptAssignment(actor);
    }
    /**
     * @brief Performs the declineAssignment operation.
     * @param actor Parameter used by this operation.
     */
    void declineAssignment(Participant& actor) override {
        if (check(actor, "declineAssignment")) real_->declineAssignment(actor);
    }
    /**
     * @brief Performs the complete operation.
     * @param result Parameter used by this operation.
     * @param actor Parameter used by this operation.
     */
    void complete(WorkflowData result, Participant& actor) override {
        if (check(actor, "complete")) real_->complete(result, actor);
    }
    /**
     * @brief Performs the reject operation.
     * @param reason Parameter used by this operation.
     * @param actor Parameter used by this operation.
     */
    void reject(const std::string& reason, Participant& actor) override {
        if (check(actor, "reject")) real_->reject(reason, actor);
    }

    // No actor available to check forwarded directly.
    /**
     * @brief Performs the makeAvailable operation.
     */
    void makeAvailable() override { real_->makeAvailable(); }
    /**
     * @brief Performs the fail operation.
     * @param reason Parameter used by this operation.
     */
    void fail(const std::string& reason) override { real_->fail(reason); }
    /**
     * @brief Performs the escalate operation.
     */
    void escalate() override { real_->escalate(); }
    /**
     * @brief Performs the hold operation.
     */
    void hold() override { real_->hold(); }
    /**
     * @brief Performs the release operation.
     */
    void release() override { real_->release(); }
    /**
     * @brief Performs the stateName operation.
     * @return The result of the operation.
     */
    std::string stateName() const override { return real_->stateName(); }

    /**
     * @brief Performs the start operation.
     */
    void start() override { real_->start(); }
    /**
     * @brief Performs the cancel operation.
     */
    void cancel() override { real_->cancel(); }
    /**
     * @brief Performs the isComplete operation.
     * @return The result of the operation.
     */
    bool isComplete() const override { return real_->isComplete(); }
    /**
     * @brief Performs the accept operation.
     * @param v Parameter used by this operation.
     */
    void accept(WorkNodeVisitor& v) override { real_->accept(v); }
    /**
     * @brief Performs the createIterator operation.
     * @return The result of the operation.
     */
    WorkNodeIterator* createIterator() override { return real_->createIterator(); }
    /**
     * @brief Performs the createReadyIterator operation.
     * @return The result of the operation.
     */
    WorkNodeIterator* createReadyIterator() override { return real_->createReadyIterator(); }
    /**
     * @brief Performs the captureState operation.
     * @param out Parameter used by this operation.
     */
    void captureState(std::map<std::string, std::string>& out) const override { real_->captureState(out); }
    /**
     * @brief Performs the restoreState operation.
     * @param in Parameter used by this operation.
     */
    void restoreState(const std::map<std::string, std::string>& in) override { real_->restoreState(in); }
    /**
     * @brief Performs the clone operation.
     * @return The result of the operation.
     */
    WorkNode* clone() const override { return real_->clone(); }

private:
    WorkItem* real_;
    const AccessPolicy* policy_;
    WorkflowEventPublisher* events_;
 
    // True if any of the actor's roles is permitted to perform `operation`.
    // Otherwise publishes ACCESS_DENIED (if a publisher was supplied) and
    // returns false, and the caller then skips the delegation.
    bool check(const Participant& actor, const std::string& operation) {
        for (const Role* role : actor.getRoles()) {
            if (role && policy_ && policy_->isAllowed(*role, operation)) return true;
        }
        if (events_) {
            WorkflowData payload;
            payload.set("operation", operation);
            payload.set("actor", actor.getId());
            // The proxy does not know which workflow instance it belongs to, so
            // instanceId is left empty; fill it in if the engine can supply it.
            WorkflowEvent denied(EventType::ACCESS_DENIED, getId(), "", payload);
            events_->publish(denied);
        }
        return false;
    }
};

#endif
