#ifndef HUMANTASK_H
#define HUMANTASK_H

#include "Task.h"
#include "WorkNodeVisitor.h"

class HumanTask : public Task {
public:
    HumanTask(std::string id, std::string name, Role* requiredRole = nullptr)
        : Task(std::move(id), std::move(name), requiredRole) {}

    // Visitor: a visitor sees this item once, as a WorkItem (WorkNodeVisitor
    // declares visit(WorkItem&)).
    void accept(WorkNodeVisitor& v) override { v.visit(*this); }

    // Prototype: copies the definition, starts fresh in CreatedState.
    HumanTask* clone() const override { return new HumanTask(*this); }
};

#endif
