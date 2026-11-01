/**
 * @file HumanTask.h
 * @brief Declares the project interfaces and types.
 */

#ifndef HUMANTASK_H
#define HUMANTASK_H

#include "Task.h"
#include "WorkNodeVisitor.h"
#include "WorkNode.h"

/**
 * @brief Represents the HumanTask component of the workflow engine.
 */
class HumanTask : public Task {
public:
    /**
     * @brief Performs the HumanTask operation.
     * @param id Parameter used by this operation.
     * @param name Parameter used by this operation.
     * @param requiredRole Parameter used by this operation.
     */
    HumanTask(std::string id, std::string name, Role* requiredRole = nullptr)
        : Task(std::move(id), std::move(name), requiredRole) {}

    // Visitor: a visitor sees this item once, as a WorkItem (WorkNodeVisitor
    // declares visit(WorkItem&)).
    /**
     * @brief Performs the accept operation.
     * @param v Parameter used by this operation.
     */
    void accept(WorkNodeVisitor& v) override { v.visit(*this); }

    // Prototype: copies the definition, starts fresh in CreatedState.
    /**
     * @brief Performs the clone operation.
     * @return The result of the operation.
     */
    HumanTask* clone() const override { return new HumanTask(*this); }
};

#endif
