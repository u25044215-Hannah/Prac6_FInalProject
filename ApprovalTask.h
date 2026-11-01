/**
 * @file ApprovalTask.h
 * @brief Declares the project interfaces and types.
 */

#ifndef APPROVALTASK_H
#define APPROVALTASK_H

#include "HumanTask.h"
#include "WorkNode.h"

class ApprovalHandler; // Chain of Responsibility (rules and decisions)
/**
 * @brief Represents the ApprovalTask component of the workflow engine.
 */
class ApprovalTask : public HumanTask {
public:
    /**
     * @brief Performs the ApprovalTask operation.
     * @param id Parameter used by this operation.
     * @param name Parameter used by this operation.
     * @param approvals Parameter used by this operation.
     * @param requiredRole Parameter used by this operation.
     */
    ApprovalTask(std::string id, std::string name, ApprovalHandler* approvals,
                 Role* requiredRole = nullptr)
        : HumanTask(std::move(id), std::move(name), requiredRole),
          approvals_(approvals) {}

    /**
     * @brief Performs the complete operation.
     * @param result Parameter used by this operation.
     * @param actor Parameter used by this operation.
     */
    void complete(WorkflowData result, Participant& actor) override;

    /**
     * @brief Performs the clone operation.
     * @return The result of the operation.
     */
    ApprovalTask* clone() const override { return new ApprovalTask(*this); }

private:
    ApprovalHandler* approvals_;
};

#endif