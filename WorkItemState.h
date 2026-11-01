/**
 * @file WorkItemState.h
 * @brief Declares the project interfaces and types.
 */


#ifndef WORKITEMSTATE_H
#define WORKITEMSTATE_H

#include <string>

#include "Participant.h"
#include "WorkflowData.h"
#include "IllegalTransitionException.h"

// Forward declaration to avoid circular dependency with Task.h
class Task;

class WorkItemState;

// Rebuild a state from its name when restoring a checkpoint.
// Returns nullptr for an unknown state name.
WorkItemState* makeStateByName(const std::string& stateName);

/**
 * @brief Represents the WorkItemState component of the workflow engine.
 */
class WorkItemState {
public:
    /**
     * @brief Performs the WorkItemState operation.
     */
    virtual ~WorkItemState() = default;

    /**
     * @brief Performs the makeAvailable operation.
     * @param t Parameter used by this operation.
     */
    virtual void makeAvailable(Task& t);
    /**
     * @brief Performs the assign operation.
     * @param t Parameter used by this operation.
     * @param p Parameter used by this operation.
     */
    virtual void assign(Task& t, Participant& p);
    /**
     * @brief Performs the acceptAssignment operation.
     * @param t Parameter used by this operation.
     */
    virtual void acceptAssignment(Task& t);
    /**
     * @brief Performs the declineAssignment operation.
     * @param t Parameter used by this operation.
     */
    virtual void declineAssignment(Task& t);
    /**
     * @brief Performs the start operation.
     * @param t Parameter used by this operation.
     */
    virtual void start(Task& t);
    /**
     * @brief Performs the complete operation.
     * @param t Parameter used by this operation.
     * @param r Parameter used by this operation.
     */
    virtual void complete(Task& t, WorkflowData r);
    /**
     * @brief Performs the fail operation.
     * @param t Parameter used by this operation.
     */
    virtual void fail(Task& t);
    /**
     * @brief Performs the reject operation.
     * @param t Parameter used by this operation.
     */
    virtual void reject(Task& t);
    /**
     * @brief Performs the cancel operation.
     * @param t Parameter used by this operation.
     */
    virtual void cancel(Task& t);
    /**
     * @brief Performs the escalate operation.
     * @param t Parameter used by this operation.
     */
    virtual void escalate(Task& t);
    /**
     * @brief Performs the hold operation.
     * @param t Parameter used by this operation.
     */
    virtual void hold(Task& t);
    /**
     * @brief Performs the release operation.
     * @param t Parameter used by this operation.
     */
    virtual void release(Task& t);

    /**
     * @brief Performs the name operation.
     * @return The result of the operation.
     */
    virtual std::string name() const = 0;
    /**
     * @brief Performs the isFinal operation.
     * @return The result of the operation.
     */
    virtual bool isFinal() const { return false; }

protected:
    [[noreturn]]
    void reject_transition(const std::string& operation) const;
};

/**
 * @brief Represents the CreatedState component of the workflow engine.
 */
class CreatedState : public WorkItemState {
public:
    /**
     * @brief Performs the makeAvailable operation.
     * @param t Parameter used by this operation.
     */
    void makeAvailable(Task& t) override;
    /**
     * @brief Performs the name operation.
     * @return The result of the operation.
     */
    std::string name() const override { return "Created"; }
};

/**
 * @brief Represents the AvailableState component of the workflow engine.
 */
class AvailableState : public WorkItemState {
public:
    /**
     * @brief Performs the assign operation.
     * @param t Parameter used by this operation.
     * @param p Parameter used by this operation.
     */
    void assign(Task& t, Participant& p) override;
    /**
     * @brief Performs the hold operation.
     * @param t Parameter used by this operation.
     */
    void hold(Task& t) override;
    /**
     * @brief Performs the cancel operation.
     * @param t Parameter used by this operation.
     */
    void cancel(Task& t) override;

    /**
     * @brief Performs the name operation.
     * @return The result of the operation.
     */
    std::string name() const override { return "Available"; }
};

/**
 * @brief Represents the AssignedState component of the workflow engine.
 */
class AssignedState : public WorkItemState {
public:
    /**
     * @brief Performs the acceptAssignment operation.
     * @param t Parameter used by this operation.
     */
    void acceptAssignment(Task& t) override;
    /**
     * @brief Performs the declineAssignment operation.
     * @param t Parameter used by this operation.
     */
    void declineAssignment(Task& t) override;
    /**
     * @brief Performs the start operation.
     * @param t Parameter used by this operation.
     */
    void start(Task& t) override;
    /**
     * @brief Performs the escalate operation.
     * @param t Parameter used by this operation.
     */
    void escalate(Task& t) override;
    /**
     * @brief Performs the cancel operation.
     * @param t Parameter used by this operation.
     */
    void cancel(Task& t) override;

    /**
     * @brief Performs the name operation.
     * @return The result of the operation.
     */
    std::string name() const override { return "Assigned"; }
};

/**
 * @brief Represents the InProgressState component of the workflow engine.
 */
class InProgressState : public WorkItemState {
public:
    /**
     * @brief Performs the complete operation.
     * @param t Parameter used by this operation.
     * @param r Parameter used by this operation.
     */
    void complete(Task& t, WorkflowData r) override;
    /**
     * @brief Performs the fail operation.
     * @param t Parameter used by this operation.
     */
    void fail(Task& t) override;
    /**
     * @brief Performs the reject operation.
     * @param t Parameter used by this operation.
     */
    void reject(Task& t) override;
    /**
     * @brief Performs the hold operation.
     * @param t Parameter used by this operation.
     */
    void hold(Task& t) override;
    /**
     * @brief Performs the escalate operation.
     * @param t Parameter used by this operation.
     */
    void escalate(Task& t) override;
    /**
     * @brief Performs the cancel operation.
     * @param t Parameter used by this operation.
     */
    void cancel(Task& t) override;

    /**
     * @brief Performs the name operation.
     * @return The result of the operation.
     */
    std::string name() const override { return "InProgress"; }
};

/**
 * @brief Represents the OnHoldState component of the workflow engine.
 */
class OnHoldState : public WorkItemState {
public:
    /**
     * @brief Performs the release operation.
     * @param t Parameter used by this operation.
     */
    void release(Task& t) override;
    /**
     * @brief Performs the cancel operation.
     * @param t Parameter used by this operation.
     */
    void cancel(Task& t) override;

    /**
     * @brief Performs the name operation.
     * @return The result of the operation.
     */
    std::string name() const override { return "OnHold"; }
};

/**
 * @brief Represents the EscalatedState component of the workflow engine.
 */
class EscalatedState : public WorkItemState {
public:
    /**
     * @brief Performs the assign operation.
     * @param t Parameter used by this operation.
     * @param p Parameter used by this operation.
     */
    void assign(Task& t, Participant& p) override;
    /**
     * @brief Performs the fail operation.
     * @param t Parameter used by this operation.
     */
    void fail(Task& t) override;
    /**
     * @brief Performs the cancel operation.
     * @param t Parameter used by this operation.
     */
    void cancel(Task& t) override;

    /**
     * @brief Performs the name operation.
     * @return The result of the operation.
     */
    std::string name() const override { return "Escalated"; }
};

/**
 * @brief Represents the CompletedState component of the workflow engine.
 */
class CompletedState : public WorkItemState {
public:
    /**
     * @brief Performs the name operation.
     * @return The result of the operation.
     */
    std::string name() const override { return "Completed"; }
    /**
     * @brief Performs the isFinal operation.
     * @return The result of the operation.
     */
    bool isFinal() const override { return true; }
};

/**
 * @brief Represents the RejectedState component of the workflow engine.
 */
class RejectedState : public WorkItemState {
public:
    /**
     * @brief Performs the name operation.
     * @return The result of the operation.
     */
    std::string name() const override { return "Rejected"; }
    /**
     * @brief Performs the isFinal operation.
     * @return The result of the operation.
     */
    bool isFinal() const override { return true; }
};

/**
 * @brief Represents the CancelledState component of the workflow engine.
 */
class CancelledState : public WorkItemState {
public:
    /**
     * @brief Performs the name operation.
     * @return The result of the operation.
     */
    std::string name() const override { return "Cancelled"; }
    /**
     * @brief Performs the isFinal operation.
     * @return The result of the operation.
     */
    bool isFinal() const override { return true; }
};

/**
 * @brief Represents the FailedState component of the workflow engine.
 */
class FailedState : public WorkItemState {
public:
    /**
     * @brief Performs the name operation.
     * @return The result of the operation.
     */
    std::string name() const override { return "Failed"; }
    /**
     * @brief Performs the isFinal operation.
     * @return The result of the operation.
     */
    bool isFinal() const override { return true; }
};

#endif // WORKITEMSTATE_H
