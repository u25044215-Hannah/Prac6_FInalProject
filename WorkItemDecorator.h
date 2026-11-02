/**
 * @file WorkItemDecorator.h
 * @brief Declares the project interfaces and types.
 */

#ifndef WORKITEMDECORATOR_H
#define WORKITEMDECORATOR_H

#include <vector>
#include "WorkItem.h"
#include "WorkflowData.h"
#include "Condition.h"
#include "ActionHistory.h"

/**
 * @brief Represents the WorkItemDecorator component of the workflow engine.
 */
class WorkItemDecorator : public WorkItem {
public:
    /**
     * @brief Performs the WorkItemDecorator operation.
     * @param inner Parameter used by this operation.
     * @return The result of the operation.
     */
    explicit WorkItemDecorator(WorkItem* inner)
        : WorkItem(inner->getId(), inner->getName()), inner_(inner) {}

    /**
     * @brief Performs the WorkItemDecorator operation.
     */
    ~WorkItemDecorator() override { delete inner_; }

    /**
     * @brief Performs the makeAvailable operation.
     */
    void makeAvailable() override { inner_->makeAvailable(); }
    /**
     * @brief Performs the assign operation.
     * @param p Parameter used by this operation.
     * @param actor Parameter used by this operation.
     */
    void assign(Participant& p, Participant& actor) override { inner_->assign(p, actor); }
    /**
     * @brief Performs the acceptAssignment operation.
     * @param actor Parameter used by this operation.
     */
    void acceptAssignment(Participant& actor) override { inner_->acceptAssignment(actor); }
    /**
     * @brief Performs the declineAssignment operation.
     * @param actor Parameter used by this operation.
     */
    void declineAssignment(Participant& actor) override { inner_->declineAssignment(actor); }
    /**
     * @brief Performs the complete operation.
     * @param result Parameter used by this operation.
     * @param actor Parameter used by this operation.
     */
    void complete(WorkflowData result, Participant& actor) override { inner_->complete(result, actor); }
    /**
     * @brief Performs the fail operation.
     * @param reason Parameter used by this operation.
     */
    void fail(const std::string& reason) override { inner_->fail(reason); }
    /**
     * @brief Performs the reject operation.
     * @param reason Parameter used by this operation.
     * @param actor Parameter used by this operation.
     */
    void reject(const std::string& reason, Participant& actor) override { inner_->reject(reason, actor); }
    /**
     * @brief Performs the escalate operation.
     */
    void escalate() override { inner_->escalate(); }
    /**
     * @brief Performs the hold operation.
     */
    void hold() override { inner_->hold(); }
    /**
     * @brief Performs the release operation.
     */
    void release() override { inner_->release(); }
    /**
     * @brief Performs the stateName operation.
     * @return The result of the operation.
     */
    std::string stateName() const override { return inner_->stateName(); }

    /**
     * @brief Performs the start operation.
     */
    void start() override { inner_->start(); }
    /**
     * @brief Performs the cancel operation.
     */
    void cancel() override { inner_->cancel(); }
    /**
     * @brief Performs the isComplete operation.
     * @return The result of the operation.
     */
    bool isComplete() const override { return inner_->isComplete(); }
    // Visitor and iterator factories are forwarded so a visitor or iterator
    // reaches the wrapped item exactly once (the decorator is not a second node).
    // The iterator yields the inner item, i.e. the undecorated Task.
    /**
     * @brief Performs the accept operation.
     * @param v Parameter used by this operation.
     */
    void accept(WorkNodeVisitor& v) override { inner_->accept(v); }
    /**
     * @brief Performs the createIterator operation.
     * @return The result of the operation.
     */
    WorkNodeIterator* createIterator() override { return inner_->createIterator(); }
    /**
     * @brief Performs the createReadyIterator operation.
     * @return The result of the operation.
     */
    WorkNodeIterator* createReadyIterator() override { return inner_->createReadyIterator(); }
    /**
     * @brief Performs the captureState operation.
     * @param out Parameter used by this operation.
     */
    void captureState(std::map<std::string, std::string>& out) const override { inner_->captureState(out); }
    /**
     * @brief Performs the restoreState operation.
     * @param in Parameter used by this operation.
     */
    void restoreState(const std::map<std::string, std::string>& in) override { inner_->restoreState(in); }
    // Returns the inner item's clone, WITHOUT re-applying the decorator layers.
    /**
     * @brief Performs the clone operation.
     * @return The result of the operation.
     */
    WorkNode* clone() const override { return inner_->clone(); }

protected:
    WorkItem* inner_;
};
/**
 * @brief Represents the NotificationDecorator component of the workflow engine.
 */
class NotificationDecorator : public WorkItemDecorator {
public:
    /**
     * @brief Performs the NotificationDecorator operation.
     * @param inner Parameter used by this operation.
     * @param notifyWhen Parameter used by this operation.
     */
    NotificationDecorator(WorkItem* inner, Condition notifyWhen)
        : WorkItemDecorator(inner), notifyWhen_(std::move(notifyWhen)) {}

    /**
     * @brief Performs the complete operation.
     * @param result Parameter used by this operation.
     * @param actor Parameter used by this operation.
     */
    void complete(WorkflowData result, Participant& actor) override;
    /**
     * @brief Performs the fail operation.
     * @param reason Parameter used by this operation.
     */
    void fail(const std::string& reason) override;

private:
    Condition notifyWhen_;
};
/**
 * @brief Represents the ValidationDecorator component of the workflow engine.
 */
class ValidationDecorator : public WorkItemDecorator {
public:
    /**
     * @brief Performs the ValidationDecorator operation.
     * @param inner Parameter used by this operation.
     * @param rules Parameter used by this operation.
     */
    ValidationDecorator(WorkItem* inner, std::vector<Condition> rules)
        : WorkItemDecorator(inner), rules_(std::move(rules)) {}

    /**
     * @brief Performs the complete operation.
     * @param result Parameter used by this operation.
     * @param actor Parameter used by this operation.
     */
    void complete(WorkflowData result, Participant& actor) override;

private:
    std::vector<Condition> rules_;
};
/**
 * @brief Represents the PriorityDecorator component of the workflow engine.
 */
class PriorityDecorator : public WorkItemDecorator {
public:
    /**
     * @brief Performs the PriorityDecorator operation.
     * @param inner Parameter used by this operation.
     * @param priority Parameter used by this operation.
     * @param targetCompletionSeconds Parameter used by this operation.
     */
    PriorityDecorator(WorkItem* inner, std::string priority, int targetCompletionSeconds)
        : WorkItemDecorator(inner), priority_(std::move(priority)),
        targetCompletionSeconds_(targetCompletionSeconds) {}

    /**
     * @brief Performs the makeAvailable operation.
     */
    void makeAvailable() override;
    /**
     * @brief Performs the assign operation.
     * @param p Parameter used by this operation.
     * @param actor Parameter used by this operation.
     */
    void assign(Participant& p, Participant& actor) override;

private:
    std::string priority_;
    int targetCompletionSeconds_;
};

/**
 * @brief Represents the AuditDecorator component of the workflow engine.
 */
class AuditDecorator : public WorkItemDecorator {
public:
    /**
     * @brief Performs the AuditDecorator operation.
     * @param inner Parameter used by this operation.
     * @param history Parameter used by this operation.
     */
    AuditDecorator(WorkItem* inner, ActionHistory* history)
        : WorkItemDecorator(inner), history_(history) {}

    /**
     * @brief Performs the makeAvailable operation.
     */
    void makeAvailable() override;
    /**
     * @brief Performs the assign operation.
     * @param p Parameter used by this operation.
     * @param actor Parameter used by this operation.
     */
    void assign(Participant& p, Participant& actor) override;
    /**
     * @brief Performs the acceptAssignment operation.
     * @param actor Parameter used by this operation.
     */
    void acceptAssignment(Participant& actor) override;
    /**
     * @brief Performs the declineAssignment operation.
     * @param actor Parameter used by this operation.
     */
    void declineAssignment(Participant& actor) override;
    /**
     * @brief Performs the complete operation.
     * @param result Parameter used by this operation.
     * @param actor Parameter used by this operation.
     */
    void complete(WorkflowData result, Participant& actor) override;
    /**
     * @brief Performs the fail operation.
     * @param reason Parameter used by this operation.
     */
    void fail(const std::string& reason) override;
    /**
     * @brief Performs the reject operation.
     * @param reason Parameter used by this operation.
     * @param actor Parameter used by this operation.
     */
    void reject(const std::string& reason, Participant& actor) override;
    /**
     * @brief Performs the escalate operation.
     */
    void escalate() override;
    /**
     * @brief Performs the hold operation.
     */
    void hold() override;
    /**
     * @brief Performs the release operation.
     */
    void release() override;

private:
    ActionHistory* history_;
};

#endif