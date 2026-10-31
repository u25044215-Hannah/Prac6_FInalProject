#ifndef APPROVALTASK_H
#define APPROVALTASK_H

#include "HumanTask.h"
#include "WorkNode.h"

class ApprovalHandler; // Chain of Responsibility (rules and decisions)
class ApprovalTask : public HumanTask {
public:
    ApprovalTask(std::string id, std::string name, ApprovalHandler* approvals,
                 Role* requiredRole = nullptr)
        : HumanTask(std::move(id), std::move(name), requiredRole),
          approvals_(approvals) {}

    void complete(WorkflowData result, Participant& actor) override;

    ApprovalTask* clone() const override { return new ApprovalTask(*this); }

private:
    ApprovalHandler* approvals_;
};

#endif