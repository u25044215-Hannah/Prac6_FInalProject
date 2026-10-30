#ifndef ACCESSCONTROLPROXY_H
#define ACCESSCONTROLPROXY_H

#include "WorkItem.h"
#include "AccessPolicy.h"
#include "Participant.h"
#include "WorkflowData.h"
#include "include/WorkflowEvent.h"
#include "include/WorkflowEventPublisher.h"
#include "Role.h"


class AccessControlProxy : public WorkItem {
public:
    AccessControlProxy(WorkItem* real, const AccessPolicy* policy, WorkflowEventPublisher* events = nullptr)
        : WorkItem(real->getId(), real->getName()), real_(real), policy_(policy), events_(events) {}

    ~AccessControlProxy() override { delete real_; }

    void assign(Participant& p, Participant& actor) override {
        if (check(actor, "assign")) real_->assign(p, actor);
    }
    void acceptAssignment(Participant& actor) override {
        if (check(actor, "acceptAssignment")) real_->acceptAssignment(actor);
    }
    void declineAssignment(Participant& actor) override {
        if (check(actor, "declineAssignment")) real_->declineAssignment(actor);
    }
    void complete(WorkflowData result, Participant& actor) override {
        if (check(actor, "complete")) real_->complete(result, actor);
    }
    void reject(const std::string& reason, Participant& actor) override {
        if (check(actor, "reject")) real_->reject(reason, actor);
    }

    // No actor available to check forwarded directly.
    void makeAvailable() override { real_->makeAvailable(); }
    void fail(const std::string& reason) override { real_->fail(reason); }
    void escalate() override { real_->escalate(); }
    void hold() override { real_->hold(); }
    void release() override { real_->release(); }
    std::string stateName() const override { return real_->stateName(); }

    void start() override { real_->start(); }
    void cancel() override { real_->cancel(); }
    bool isComplete() const override { return real_->isComplete(); }
    void accept(WorkNodeVisitor& v) override { real_->accept(v); }
    WorkNodeIterator* createIterator() override { return real_->createIterator(); }
    WorkNodeIterator* createReadyIterator() override { return real_->createReadyIterator(); }
    void captureState(std::map<std::string, std::string>& out) const override { real_->captureState(out); }
    void restoreState(const std::map<std::string, std::string>& in) override { real_->restoreState(in); }
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