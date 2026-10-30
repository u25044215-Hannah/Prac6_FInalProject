#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>
#include "WorkItem.h"
#include "WorkItemState.h"
#include "DependencySet.h"
#include "AssignmentStrategy.h"
#include "DepthFirstIterator.h"
#include "ReadyItemIterator.h"
#include "include/WorkflowEventPublisher.h"

class Task : public WorkItem {
public:
    Task(std::string id, std::string name, Role* requiredRole = nullptr)
        : WorkItem(std::move(id), std::move(name)),
        state(new CreatedState()),
        requiredRole(requiredRole) {}

    // Task owns its WorkItemState*, so the compiler-generated copy would leave
    // two Tasks pointing at (and deleting) the same state. A copy is a clone:
    // it takes the definition (id/name via WorkNode's copy constructor, role,
    // skills, dependencies, policy) and starts fresh in CreatedState with no
    // assignee or result. WorkNode's copy constructor also clears the parent.
    Task(const Task& other)
        : WorkItem(other),
        state(new CreatedState()),
        dependencies(other.dependencies),
        requiredRole(other.requiredRole),
        requiredSkills(other.requiredSkills),
        assignmentPolicy(other.assignmentPolicy),
        events(other.events) {}

    ~Task() override { delete state; }

    // Iterator factories (WorkNode declares them pure). A leaf has no children,
    // so these iterate over just this item, consistent with DepthFirstIterator
    // yielding its starting node first. The caller owns the returned iterator.
    WorkNodeIterator* createIterator() override { return new DepthFirstIterator(this); }
    WorkNodeIterator* createReadyIterator() override { return new ReadyItemIterator(this); }

    // Read access used by the assignment strategies (RoleAssignment,
    // SkillAssignment). Neither is drawn on Task in Figure 5.3.
    Role* getRequiredRole() const { return requiredRole; }
    const std::vector<std::string>& getRequiredSkills() const { return requiredSkills; }
    void addRequiredSkill(const std::string& skill) { requiredSkills.push_back(skill); }

    
    void setState(WorkItemState* s) { delete state; state = s; }
    const DependencySet& getDependencies() const { return dependencies; }
    void addDependency(const std::string& nodeId) { dependencies.add(nodeId); }

    
    void makeAvailable() override { state->makeAvailable(*this); }
    virtual Task* clone() const override = 0;


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


    void start() override { state->start(*this); }
    void cancel() override { state->cancel(*this); }
    bool isComplete() const override { return state->isFinal(); }
    void setAssignee(Participant& p) { assignee = &p; }
    void clearAssignee() { assignee = nullptr; }
    Participant* getAssignee() const { return assignee; }
    void setResult(const WorkflowData& r) { result = r; }

protected:
    WorkItemState* state;
    DependencySet dependencies;
    Role* requiredRole;
    std::vector<std::string> requiredSkills;
    Participant* assignee = nullptr;
    AssignmentStrategy* assignmentPolicy = nullptr;
    WorkflowEventPublisher* events = nullptr;
    WorkflowData result;
    std::string lastFailureReason;
    std::string lastRejectionReason;
};

#endif