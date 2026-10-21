#ifndef WORKITEMDECORATOR_H
#define WORKITEMDECORATOR_H

#include <vector>
#include "WorkItem.h"
#include "Forward.h"
class WorkItemDecorator : public WorkItem {
public:
    explicit WorkItemDecorator(WorkItem* inner)
        : WorkItem(inner->getId(), inner->getName()), inner_(inner) {}

    ~WorkItemDecorator() override { delete inner_; }

    void makeAvailable() override { inner_->makeAvailable(); }
    void assign(Participant& p, Participant& actor) override { inner_->assign(p, actor); }
    void acceptAssignment(Participant& actor) override { inner_->acceptAssignment(actor); }
    void declineAssignment(Participant& actor) override { inner_->declineAssignment(actor); }
    void complete(WorkflowData result, Participant& actor) override { inner_->complete(result, actor); }
    void fail(const std::string& reason) override { inner_->fail(reason); }
    void reject(const std::string& reason, Participant& actor) override { inner_->reject(reason, actor); }
    void escalate() override { inner_->escalate(); }
    void hold() override { inner_->hold(); }
    void release() override { inner_->release(); }
    std::string stateName() const override { return inner_->stateName(); }

    void start() override { inner_->start(); }
    void cancel() override { inner_->cancel(); }
    bool isComplete() const override { return inner_->isComplete(); }
    WorkNode* clone() const override { return inner_->clone(); } // each concrete decorator should re-wrap; see note below

protected:
    WorkItem* inner_;
};
class NotificationDecorator : public WorkItemDecorator {
public:
    NotificationDecorator(WorkItem* inner, Condition notifyWhen)
        : WorkItemDecorator(inner), notifyWhen_(std::move(notifyWhen)) {}

    void complete(WorkflowData result, Participant& actor) override;
    void fail(const std::string& reason) override;

private:
    Condition notifyWhen_;
};
class ValidationDecorator : public WorkItemDecorator {
public:
    ValidationDecorator(WorkItem* inner, std::vector<Condition> rules)
        : WorkItemDecorator(inner), rules_(std::move(rules)) {}

    void complete(WorkflowData result, Participant& actor) override;

private:
    std::vector<Condition> rules_;
};
class PriorityDecorator : public WorkItemDecorator {
public:
    PriorityDecorator(WorkItem* inner, std::string priority, int targetCompletionSeconds)
        : WorkItemDecorator(inner), priority_(std::move(priority)),
        targetCompletionSeconds_(targetCompletionSeconds) {}

    void makeAvailable() override;
    void assign(Participant& p, Participant& actor) override;

private:
    std::string priority_;
    int targetCompletionSeconds_;
};

class AuditDecorator : public WorkItemDecorator {
public:
    AuditDecorator(WorkItem* inner, ActionHistory* history)
        : WorkItemDecorator(inner), history_(history) {}

    void makeAvailable() override;
    void assign(Participant& p, Participant& actor) override;
    void acceptAssignment(Participant& actor) override;
    void declineAssignment(Participant& actor) override;
    void complete(WorkflowData result, Participant& actor) override;
    void fail(const std::string& reason) override;
    void reject(const std::string& reason, Participant& actor) override;
    void escalate() override;
    void hold() override;
    void release() override;

private:
    ActionHistory* history_;
};

#endif
