#ifndef ACCESSCONTROLPROXY_H
#define ACCESSCONTROLPROXY_H

#include "WorkItem.h"
#include "AccessPolicy.h"
#include "Participant.h"
#include "WorkflowData.h"
#include "WorkflowEvent.h"
#include "WorkflowEventPublisher.h"
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
    WorkNode* clone() const override { return real_->clone(); }

private:
    WorkItem* real_;
    const AccessPolicy* policy_;
    WorkflowEventPublisher* events_;
 
    bool check(const Participant& actor, const std::string& operation) {}
};

#endif
