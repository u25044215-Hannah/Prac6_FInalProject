#ifndef AUTOMATEDTASK_H
#define AUTOMATEDTASK_H

#include "Task.h"
#include "WorkNodeVisitor.h"
class AutomatedTask : public Task {
public:
    AutomatedTask(std::string id, std::string name, Role* requiredRole = nullptr)
        : Task(std::move(id), std::move(name), requiredRole) {}

    virtual void execute() {
        start();
        state_->complete(*this, WorkflowData());
    }

    void accept(WorkNodeVisitor& v) override { v.visit(*this); }

    AutomatedTask* clone() const override { return new AutomatedTask(*this); }
};

#endif
