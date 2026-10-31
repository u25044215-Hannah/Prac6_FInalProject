
#ifndef TASK_H
#define TASK_H

#include <map>
#include <string>
#include <vector>
#include <utility>

#include "WorkItem.h"
#include "WorkItemState.h"
#include "DependencySet.h"
#include "DepthFirstIterator.h"
#include "ReadyItemIterator.h"
#include "include/WorkflowEventPublisher.h"

// Forward declaration prevents circular dependency
// between Task.h and AssignmentStrategy.h
class AssignmentStrategy;

class Task : public WorkItem {
public:

    Task(std::string id, std::string name,
         Role* requiredRole = nullptr)
        : WorkItem(std::move(id), std::move(name)),
          state(new CreatedState()),
          requiredRole(requiredRole) {}

    // Prototype: copy the task definition and reset its state.
    Task(const Task& other)
        : WorkItem(other),
          state(new CreatedState()),
          dependencies(other.dependencies),
          requiredRole(other.requiredRole),
          requiredSkills(other.requiredSkills),
          assignmentPolicy(other.assignmentPolicy),
          events(other.events) {}

    Task& operator=(const Task&) = delete;

    ~Task() override {
        delete state;
    }

    // Iterator Pattern
    WorkNodeIterator* createIterator() override {
        return new DepthFirstIterator(this);
    }

    WorkNodeIterator* createReadyIterator() override {
        return new ReadyItemIterator(this);
    }

    // Assignment Strategy
    Role* getRequiredRole() const {
        return requiredRole;
    }

    const std::vector<std::string>& getRequiredSkills() const {
        return requiredSkills;
    }

    void addRequiredSkill(const std::string& skill) {
        requiredSkills.push_back(skill);
    }

    // Memento Pattern
    void captureState(
        std::map<std::string, std::string>& out
    ) const override {
        out[getId()] = state->name();
    }

    void restoreState(
        const std::map<std::string, std::string>& in
    ) override {
        auto it = in.find(getId());

        if (it == in.end()) {
            return;
        }

        WorkItemState* restored =
            makeStateByName(it->second);

        if (restored != nullptr) {
            setState(restored);
        }
    }

    // State Pattern
    void setState(WorkItemState* s) {
        if (s == state) {
            return;
        }

        delete state;
        state = s;
    }

    const DependencySet& getDependencies() const {
        return dependencies;
    }

    void addDependency(const std::string& nodeId) {
        dependencies.add(nodeId);
    }

    void makeAvailable() override {
        state->makeAvailable(*this);
    }

    // Concrete subclasses implement their own clone.
    virtual Task* clone() const override = 0;

    void assign(Participant& p, Participant&) override {
        state->assign(*this, p);
    }

    void acceptAssignment(Participant&) override {
        state->acceptAssignment(*this);
    }

    void declineAssignment(Participant&) override {
        state->declineAssignment(*this);
    }

    void complete(
        WorkflowData result,
        Participant&
    ) override {
        state->complete(*this, result);
    }

    void fail(const std::string& reason) override {
        lastFailureReason = reason;
        state->fail(*this);
    }

    void reject(
        const std::string& reason,
        Participant&
    ) override {
        lastRejectionReason = reason;
        state->reject(*this);
    }

    void escalate() override {
        state->escalate(*this);
    }

    void hold() override {
        state->hold(*this);
    }

    void release() override {
        state->release(*this);
    }

    std::string stateName() const override {
        return state->name();
    }

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

    // Helpers used by state implementations
    void setAssignee(Participant& p) {
        assignee = &p;
    }

    void clearAssignee() {
        assignee = nullptr;
    }

    Participant* getAssignee() const {
        return assignee;
    }

    void setResult(const WorkflowData& r) {
        result = r;
    }

protected:
    WorkItemState* state;
    DependencySet dependencies;

    Role* requiredRole;
    std::vector<std::string> requiredSkills;

    Participant* assignee = nullptr;

    // Non-owning strategy and event publisher pointers
    AssignmentStrategy* assignmentPolicy = nullptr;
    WorkflowEventPublisher* events = nullptr;

    WorkflowData result;

    std::string lastFailureReason;
    std::string lastRejectionReason;
};

#endif // TASK_H
