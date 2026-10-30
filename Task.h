#ifndef TASK_H
#define TASK_H
#include <vector>
#include <string>
#include <utility>
#include "WorkItem.h"
#include "WorkItemState.h"
#include "Role.h"
#include "Participant.h"
#include "WorkflowData.h"
#include "DependencySet.h"
//#include "AssignmentStrategy.h"
#include "include/WorkflowEventPublisher.h"


class AssignmentStrategy;
class Task : public WorkItem {
public:

    const std::vector<std::string>& getRequiredSkills() const {
    return requiredSkills;
}

void addRequiredSkill(const std::string& skill) {
    requiredSkills.push_back(skill);
}
    Role* getRequiredRole() const {
    return requiredRole;
    }

    Task(std::string id, std::string name, Role* requiredRole = nullptr)
        : WorkItem(std::move(id), std::move(name)),
          requiredRole(requiredRole) {}

    Task(const Task& other)
        : WorkItem(other.getId(), other.getName()),
          dependencies(other.dependencies),
          requiredRole(other.requiredRole),
          requiredSkills(other.requiredSkills),
          events(other.events) {}

    Task& operator=(const Task&) = delete;

    ~Task() override { delete state; }

    //State (own listed interface)
    void setState(WorkItemState* s) {
        if (s != state) {
            delete state;
            state = s;
        }
    }
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
    return nullptr;
}

WorkNodeIterator* createReadyIterator() override {
    return nullptr;
}

// clone() remains implemented by concrete subclasses

    // Helpers concrete WorkItemState implementations call on Task
    void setAssignee(Participant& p) { assignee = &p; }
    void clearAssignee() { assignee = nullptr; }
    Participant* getAssignee() const { return assignee; }
    const Role* getRequiredRole() const;
    void setResult(const WorkflowData& r) { result = r; }

protected:
    std::vector<std::string> requiredSkills;
    WorkItemState* state;
    DependencySet dependencies;
    Role* requiredRole;
    Participant* assignee = nullptr;
    WorkflowEventPublisher* events = nullptr;
    WorkflowData result;
    std::string lastFailureReason;
    std::string lastRejectionReason;
};

#endif // TASK_H
