#ifndef WORKITEMSTATE_H
#define WORKITEMSTATE_H

#include <string>

class Task;
class WorkItemState {
public:
    virtual ~WorkItemState() = default;

    virtual void makeAvailable(Task& t);
    virtual void assign(Task& t, Participant& p);
    virtual void acceptAssignment(Task& t);
    virtual void declineAssignment(Task& t);
    virtual void start(Task& t);
    virtual void complete(Task& t, WorkflowData r);
    virtual void fail(Task& t);
    virtual void reject(Task& t);
    virtual void cancel(Task& t);
    virtual void escalate(Task& t);
    virtual void hold(Task& t);
    virtual void release(Task& t);

    virtual std::string name() const = 0;
    virtual bool isFinal() const { return false; }

protected:
    [[noreturn]] void reject_transition(const std::string& operation) const;
};

class CreatedState : public WorkItemState {
public:
    void makeAvailable(Task& t) override;
    std::string name() const override { return "Created"; }
};

class AvailableState : public WorkItemState {
public:
    void assign(Task& t, Participant& p) override;
    void hold(Task& t) override;
    void cancel(Task& t) override;
    std::string name() const override { return "Available"; }
};

class AssignedState : public WorkItemState {
public:
    void acceptAssignment(Task& t) override;
    void declineAssignment(Task& t) override;
    void start(Task& t) override;
    void escalate(Task& t) override;
    void cancel(Task& t) override;
    std::string name() const override { return "Assigned"; }
};

class InProgressState : public WorkItemState {
public:
    void complete(Task& t, WorkflowData r) override;
    void fail(Task& t) override;
    void reject(Task& t) override;
    void hold(Task& t) override;
    void escalate(Task& t) override;
    void cancel(Task& t) override;
    std::string name() const override { return "InProgress"; }
};

class OnHoldState : public WorkItemState {
public:
    void release(Task& t) override;
    void cancel(Task& t) override;
    std::string name() const override { return "OnHold"; }
};

class EscalatedState : public WorkItemState {
public:
    void assign(Task& t, Participant& p) override;
    void fail(Task& t) override;
    void cancel(Task& t) override;
    std::string name() const override { return "Escalated"; }
};

class CompletedState : public WorkItemState {
public:
    std::string name() const override { return "Completed"; }
    bool isFinal() const override { return true; }
};

class RejectedState : public WorkItemState {
public:
    std::string name() const override { return "Rejected"; }
    bool isFinal() const override { return true; }
};

class CancelledState : public WorkItemState {
public:
    std::string name() const override { return "Cancelled"; }
    bool isFinal() const override { return true; }
};

class FailedState : public WorkItemState {
public:
    std::string name() const override { return "Failed"; }
    bool isFinal() const override { return true; }
};

#endif
