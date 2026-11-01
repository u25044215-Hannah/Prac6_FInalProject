/**
 * @file Task.h
 * @brief Declares the project interfaces and types.
 */


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

/**
 * @brief Represents the Task component of the workflow engine.
 */
class Task : public WorkItem {
public:

    /**
     * @brief Performs the Task operation.
     * @param id Parameter used by this operation.
     * @param name Parameter used by this operation.
     * @param requiredRole Parameter used by this operation.
     */
    Task(std::string id, std::string name,
         Role* requiredRole = nullptr)
        : WorkItem(std::move(id), std::move(name)),
          state(new CreatedState()),
          requiredRole(requiredRole) {}

    // Prototype: copy the task definition and reset its state.
    /**
     * @brief Performs the Task operation.
     * @param other Parameter used by this operation.
     */
    Task(const Task& other)
        : WorkItem(other),
          state(new CreatedState()),
          dependencies(other.dependencies),
          requiredRole(other.requiredRole),
          requiredSkills(other.requiredSkills),
          assignmentPolicy(other.assignmentPolicy),
          events(other.events) {}

    Task& operator=(const Task&) = delete;

    /**
     * @brief Performs the Task operation.
     */
    ~Task() override {
        delete state;
    }

    // Iterator Pattern
    /**
     * @brief Performs the createIterator operation.
     * @return The result of the operation.
     */
    WorkNodeIterator* createIterator() override {
        return new DepthFirstIterator(this);
    }

    /**
     * @brief Performs the createReadyIterator operation.
     * @return The result of the operation.
     */
    WorkNodeIterator* createReadyIterator() override {
        return new ReadyItemIterator(this);
    }

    // Assignment Strategy
    /**
     * @brief Performs the getRequiredRole operation.
     * @return The result of the operation.
     */
    Role* getRequiredRole() const {
        return requiredRole;
    }

    /**
     * @brief Performs the getRequiredSkills operation.
     * @return The result of the operation.
     */
    const std::vector<std::string>& getRequiredSkills() const {
        return requiredSkills;
    }

    /**
     * @brief Performs the addRequiredSkill operation.
     * @param skill Parameter used by this operation.
     */
    void addRequiredSkill(const std::string& skill) {
        requiredSkills.push_back(skill);
    }

    // Memento Pattern
    /**
     * @brief Performs the captureState operation.
     * @param out Parameter used by this operation.
     */
    void captureState(
        std::map<std::string, std::string>& out
    ) const override {
        out[getId()] = state->name();
    }

    /**
     * @brief Performs the restoreState operation.
     * @param in Parameter used by this operation.
     */
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
    /**
     * @brief Performs the setState operation.
     * @param s Parameter used by this operation.
     */
    void setState(WorkItemState* s) {
        if (s == state) {
            return;
        }

        delete state;
        state = s;
    }

    /**
     * @brief Performs the getDependencies operation.
     * @return The result of the operation.
     */
    const DependencySet& getDependencies() const {
        return dependencies;
    }

    /**
     * @brief Performs the addDependency operation.
     * @param nodeId Parameter used by this operation.
     */
    void addDependency(const std::string& nodeId) {
        dependencies.add(nodeId);
    }

    /**
     * @brief Performs the makeAvailable operation.
     */
    void makeAvailable() override {
        state->makeAvailable(*this);
    }

    // Concrete subclasses implement their own clone.
    /**
     * @brief Performs the clone operation.
     * @return The result of the operation.
     */
    virtual Task* clone() const override = 0;

    /**
     * @brief Performs the assign operation.
     * @param p Parameter used by this operation.
     */
    void assign(Participant& p, Participant&) override {
        state->assign(*this, p);
    }

    /**
     * @brief Performs the acceptAssignment operation.
     */
    void acceptAssignment(Participant&) override {
        state->acceptAssignment(*this);
    }

    /**
     * @brief Performs the declineAssignment operation.
     */
    void declineAssignment(Participant&) override {
        state->declineAssignment(*this);
    }

    /**
     * @brief Performs the complete operation.
     * @param result Parameter used by this operation.
     */
    void complete(
        WorkflowData result,
        Participant&
    ) override {
        state->complete(*this, result);
    }

    /**
     * @brief Performs the fail operation.
     * @param reason Parameter used by this operation.
     */
    void fail(const std::string& reason) override {
        lastFailureReason = reason;
        state->fail(*this);
    }

    /**
     * @brief Performs the reject operation.
     * @param reason Parameter used by this operation.
     */
    void reject(
        const std::string& reason,
        Participant&
    ) override {
        lastRejectionReason = reason;
        state->reject(*this);
    }

    /**
     * @brief Performs the escalate operation.
     */
    void escalate() override {
        state->escalate(*this);
    }

    /**
     * @brief Performs the hold operation.
     */
    void hold() override {
        state->hold(*this);
    }

    /**
     * @brief Performs the release operation.
     */
    void release() override {
        state->release(*this);
    }

    /**
     * @brief Performs the stateName operation.
     * @return The result of the operation.
     */
    std::string stateName() const override {
        return state->name();
    }

    // WorkNode interface
    /**
     * @brief Performs the start operation.
     */
    void start() override {
        state->start(*this);
    }

    /**
     * @brief Performs the cancel operation.
     */
    void cancel() override {
        state->cancel(*this);
    }

    /**
     * @brief Performs the isComplete operation.
     * @return The result of the operation.
     */
    bool isComplete() const override {
        return state->isFinal();
    }

    // Helpers used by state implementations
    /**
     * @brief Performs the setAssignee operation.
     * @param p Parameter used by this operation.
     */
    void setAssignee(Participant& p) {
        assignee = &p;
    }

    /**
     * @brief Performs the clearAssignee operation.
     */
    void clearAssignee() {
        assignee = nullptr;
    }

    /**
     * @brief Performs the getAssignee operation.
     * @return The result of the operation.
     */
    Participant* getAssignee() const {
        return assignee;
    }

    /**
     * @brief Performs the setResult operation.
     * @param r Parameter used by this operation.
     */
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
