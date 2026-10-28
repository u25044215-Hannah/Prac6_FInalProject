#ifndef TASK_H
#define TASK_H

#include <string>
#include "WorkItem.h"
#include "WorkItemState.h"
#include "person5Forward.h"
class Task : public WorkItem {
public:
    Task(std::string id, std::string name, Role* requiredRole = nullptr)
        : WorkItem(std::move(id), std::move(name)),
        state_(new CreatedState()),
        requiredRole_(requiredRole) {}

    ~Task() override { delete state_; }

    
    void setState(WorkItemState* s) { delete state_; state_ = s; }
    const DependencySet& getDependencies() const { return dependencies_; }
    void addDependency(const std::string& nodeId) { dependencies_.add(nodeId); }

    
    void makeAvailable() override { state_->makeAvailable(*this); }

    void assign(Participant& p, Participant& ) override {
        state_->assign(*this, p);
    }
    void acceptAssignment(Participant& ) override {
        state_->acceptAssignment(*this);
    }
    void declineAssignment(Participant& ) override {
        state_->declineAssignment(*this);
    }
    void complete(WorkflowData result, Participant& ) override {
        state_->complete(*this, result);
    }
    void fail(const std::string& reason) override {
        lastFailureReason_ = reason;
        state_->fail(*this);
    }
    void reject(const std::string& reason, Participant& ) override {
        lastRejectionReason_ = reason;
        state_->reject(*this);
    }
    void escalate() override { state_->escalate(*this); }
    void hold() override { state_->hold(*this); }
    void release() override { state_->release(*this); }
    std::string stateName() const override { return state_->name(); }


    void start() override { state_->start(*this); }
    void cancel() override { state_->cancel(*this); }
    bool isComplete() const override { return state_->isFinal(); }
    void setAssignee(Participant& p) { assignee_ = &p; }
    void clearAssignee() { assignee_ = nullptr; }
    Participant* getAssignee() const { return assignee_; }
    void setResult(const WorkflowData& r) { result_ = r; }

protected:
    WorkItemState* state_;
    DependencySet dependencies_;
    Role* requiredRole_;
    Participant* assignee_ = nullptr;
    AssignmentStrategy* assignmentPolicy_ = nullptr;
    WorkflowEventPublisher* events_ = nullptr;
    WorkflowData result_;
    std::string lastFailureReason_;
    std::string lastRejectionReason_;
};

#endif
