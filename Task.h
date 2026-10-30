#ifndef TASK_H
#define TASK_H

#include <string>
#include "WorkItem.h"
#include "WorkItemState.h"
#include "Role.h"
#include "Participant.h"
#include "WorkflowData.h"
#include "DependencySet.h"
#include "AssignmentStrategy.h"
#include "include/WorkflowEventPublisher.h"
#include "EmptyWorkNodeIterator.h"



class Task : public WorkItem {
public:
    Task(std::string id, std::string name, Role* requiredRole = nullptr)
        : WorkItem(std::move(id), std::move(name)),
          state(new CreatedState()),
          requiredRole(requiredRole) {}

    Task(const Task& other)
        : WorkItem(other.getId(), other.getName()),
          state(new CreatedState()),
          dependencies(other.dependencies),
          requiredRole(other.requiredRole),
          assignmentPolicy(other.assignmentPolicy),
          events(other.events) {}

    Task& operator=(const Task&) = delete;

    ~Task() override { delete state; }

    //State (own listed interface)
    void setState(WorkItemState* s) { delete state; state = s; }
    const DependencySet& getDependencies() const { return dependencies; }
    void addDependency(const std::string& nodeId) { dependencies.add(nodeId); }

    // WorkItem interface: every operation delegates to state
    void makeAvailable() override { state->makeAvailable(*this); }

    void assign(Participant& p, Participant& ) override {
        state->assign(*this, p);
    }
    void acceptAssignment(Participant& ) override {
        state->acceptAssignment(*this);
    }
    void declineAssignment(Participant& ) override {
        state->declineAssignment(*this);
    }
    void complete(WorkflowData result, Participant& ) override {
        state->complete(*this, result);
    }
    void fail(const std::string& reason) override {
        lastFailureReason = reason;
        state->fail(*this);
    }
    void reject(const std::string& reason, Participant& ) override {
        lastRejectionReason = reason;
        state->reject(*this);
    }
    void escalate() override { state->escalate(*this); }
    void hold() override { state->hold(*this); }
    void release() override { state->release(*this); }
    std::string stateName() const override { return state->name(); }

    //WorkNode interface
    // WorkNode interface
void start() override {
    state->start(*this);
}

void cancel() override {
    state->cancel(*this);
}

bool isComplete() const override {
    return state->isFinal();
}

// Iterator pattern: Task is a leaf node,
// so it has no children to iterate over.
WorkNodeIterator* createIterator() override {
    return new EmptyWorkNodeIterator();
}

WorkNodeIterator* createReadyIterator() override {
    return new EmptyWorkNodeIterator();
}

// clone() remains implemented by concrete subclasses

    // Helpers concrete WorkItemState implementations call on Task
    void setAssignee(Participant& p) { assignee = &p; }
    void clearAssignee() { assignee = nullptr; }
    Participant* getAssignee() const { return assignee; }
    void setResult(const WorkflowData& r) { result = r; }

protected:
    WorkItemState* state;
    DependencySet dependencies;
    Role* requiredRole;
    Participant* assignee = nullptr;
    AssignmentStrategy* assignmentPolicy = nullptr;
    WorkflowEventPublisher* events= nullptr;
    WorkflowData result;
    std::string lastFailureReason;
    std::string lastRejectionReason;
};

#endif
