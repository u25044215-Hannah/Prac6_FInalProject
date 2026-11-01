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

// Observer Pattern
#include "include/WorkflowEventPublisher.h"
#include "include/WorkflowEvent.h"

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

    // Prototype Pattern:
    // Copy task configuration but reset execution state
    // and Observer connections.
    Task(const Task& other)
        : WorkItem(other),
          state(new CreatedState()),
          dependencies(other.dependencies),
          requiredRole(other.requiredRole),
          requiredSkills(other.requiredSkills),
          assignmentPolicy(other.assignmentPolicy),
          events(nullptr) {}

    Task& operator=(const Task&) = delete;

    ~Task() override {
        clearRetiredStates();
        delete state;
    }

    // =====================================================
    // OBSERVER PATTERN
    // =====================================================

    // Connect this task to its workflow instance.
    // The publisher is owned elsewhere.
    void setEventPublisher(
        WorkflowEventPublisher* publisher,
        const std::string& workflowInstanceId
    ) {
        events = publisher;
        instanceId = workflowInstanceId;
    }

    // =====================================================
    // ITERATOR PATTERN
    // =====================================================

    WorkNodeIterator* createIterator() override {
        return new DepthFirstIterator(this);
    }

    WorkNodeIterator* createReadyIterator() override {
        return new ReadyItemIterator(this);
    }

    // =====================================================
    // ASSIGNMENT STRATEGY
    // =====================================================

    Role* getRequiredRole() const {
        return requiredRole;
    }

    const std::vector<std::string>& getRequiredSkills() const {
        return requiredSkills;
    }

    void addRequiredSkill(const std::string& skill) {
        requiredSkills.push_back(skill);
    }

    // =====================================================
    // MEMENTO PATTERN
    // =====================================================

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
            // Restoring a checkpoint must not publish events.
            setState(restored, false);
        }
    }

    // =====================================================
    // STATE PATTERN + OBSERVER PATTERN
    // =====================================================

    // Changes the task's state.
    //
    // Old states are kept alive while a state operation
    // is executing, preventing premature deletion.
    //
    // notifyObservers=false is used during restoration.
    void setState(
        WorkItemState* s,
        bool notifyObservers = true
    ) {
        if (s == nullptr || s == state) {
            return;
        }

        const std::string oldState = state->name();
        const std::string newState = s->name();

        WorkItemState* previousState = state;

        // Install the new state.
        state = s;

        // Delay deletion if a state operation is active.
        if (stateOperationDepth > 0) {
            retiredStates.push_back(previousState);
        } else {
            delete previousState;
        }

        // Automatically notify observers when the state changes.
        if (notifyObservers &&
            events != nullptr &&
            !instanceId.empty() &&
            oldState != newState) {

            WorkflowEvent event(
                EventType::WORK_ITEM_STATE_CHANGED,
                getId(),
                instanceId,
                WorkflowData()
            );

            events->publish(event);
        }
    }

    // =====================================================
    // DEPENDENCY MANAGEMENT
    // =====================================================

    const DependencySet& getDependencies() const {
        return dependencies;
    }

    void addDependency(const std::string& nodeId) {
        dependencies.add(nodeId);
    }

    void makeAvailable() override {
        performStateOperation([this]() {
            state->makeAvailable(*this);
        });
    }

    // =====================================================
    // PROTOTYPE PATTERN
    // =====================================================

    virtual Task* clone() const override = 0;

    // =====================================================
    // STATE PATTERN OPERATIONS
    // =====================================================

    void assign(Participant& p, Participant&) override {
        performStateOperation([this, &p]() {
            state->assign(*this, p);
        });
    }

    void acceptAssignment(Participant&) override {
        performStateOperation([this]() {
            state->acceptAssignment(*this);
        });
    }

    void declineAssignment(Participant&) override {
        performStateOperation([this]() {
            state->declineAssignment(*this);
        });
    }

    void complete(
        WorkflowData result,
        Participant&
    ) override {
        performStateOperation([this, &result]() {
            state->complete(*this, result);
        });
    }

    void fail(const std::string& reason) override {
        lastFailureReason = reason;

        performStateOperation([this]() {
            state->fail(*this);
        });
    }

    void reject(
        const std::string& reason,
        Participant&
    ) override {
        lastRejectionReason = reason;

        performStateOperation([this]() {
            state->reject(*this);
        });
    }

    void escalate() override {
        performStateOperation([this]() {
            state->escalate(*this);
        });
    }

    void hold() override {
        performStateOperation([this]() {
            state->hold(*this);
        });
    }

    void release() override {
        performStateOperation([this]() {
            state->release(*this);
        });
    }

    std::string stateName() const override {
        return state->name();
    }

    // =====================================================
    // WORKNODE INTERFACE
    // =====================================================

    void start() override {
        performStateOperation([this]() {
            state->start(*this);
        });
    }

    void cancel() override {
        performStateOperation([this]() {
            state->cancel(*this);
        });
    }

    bool isComplete() const override {
        return state->isFinal();
    }

    // =====================================================
    // HELPERS USED BY STATE IMPLEMENTATIONS
    // =====================================================

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

    // =====================================================
    // STATE OPERATION SAFETY
    // =====================================================

    // Tracks active state operations.
    int stateOperationDepth = 0;

    // Holds replaced states until operations finish.
    std::vector<WorkItemState*> retiredStates;

    // Deletes old state objects once it is safe.
    void clearRetiredStates() {
        for (WorkItemState* oldState : retiredStates) {
            delete oldState;
        }

        retiredStates.clear();
    }

    // Executes a state operation safely.
    //
    // The previous state remains alive until the
    // operation finishes, including when exceptions occur.
    template <typename Operation>
    void performStateOperation(Operation operation) {
        ++stateOperationDepth;

        try {
            operation();
        } catch (...) {
            --stateOperationDepth;

            if (stateOperationDepth == 0) {
                clearRetiredStates();
            }

            throw;
        }

        --stateOperationDepth;

        if (stateOperationDepth == 0) {
            clearRetiredStates();
        }
    }

    // =====================================================
    // TASK DATA
    // =====================================================

    WorkItemState* state;

    DependencySet dependencies;

    Role* requiredRole;

    std::vector<std::string> requiredSkills;

    Participant* assignee = nullptr;

    // Non-owning strategy pointer.
    AssignmentStrategy* assignmentPolicy = nullptr;

    // Non-owning Observer publisher pointer.
    WorkflowEventPublisher* events = nullptr;

    // Identifies the workflow instance that owns this task.
    std::string instanceId;

    WorkflowData result;

    std::string lastFailureReason;
    std::string lastRejectionReason;
};

#endif // TASK_H