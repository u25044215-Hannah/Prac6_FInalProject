/**
 * @file AutomatedTask.h
 * @brief Declares the project interfaces and types.
 */

#ifndef AUTOMATEDTASK_H
#define AUTOMATEDTASK_H

#include "Task.h"
#include "WorkNodeVisitor.h"
#include "Role.h"
#include "WorkNode.h"

/**
 * @brief Represents the AutomatedTask component of the workflow engine.
 */
class AutomatedTask : public Task {
public:
    /**
     * @brief Performs the AutomatedTask operation.
     * @param id Parameter used by this operation.
     * @param name Parameter used by this operation.
     * @param requiredRole Parameter used by this operation.
     */
    AutomatedTask(std::string id, std::string name, Role* requiredRole = nullptr)
        : Task(std::move(id), std::move(name), requiredRole) {}

    /**
     * @brief Performs the execute operation.
     */
    virtual void execute() {
        start();
        state->complete(*this, WorkflowData());
    }

    /**
     * @brief Performs the accept operation.
     * @param v Parameter used by this operation.
     */
    void accept(WorkNodeVisitor& v) override { v.visit(*this); }

    /**
     * @brief Performs the clone operation.
     * @return The result of the operation.
     */
    AutomatedTask* clone() const override { return new AutomatedTask(*this); }
};

#endif