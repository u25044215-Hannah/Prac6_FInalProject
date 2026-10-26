#include "WorkflowInstance.h"
#include "InstanceSnapshot.h"
#include "TividyException.h"
#include "WorkNode.h"
#include "WorkflowDefinition.h"

namespace {
///deep-copy the template tree so the instance never shares state with it
std::unique_ptr<WorkNode> cloneTemplate(const WorkflowDefinition& def) {
    std::unique_ptr<WorkNode> copy(def.getRoot().clone());
    if (!copy) 
    {
        throw InvalidOperationException("Cloning the template of " + def.label() + " failed");
    }
    return copy;
}
}

WorkflowInstance::WorkflowInstance(const std::string& id, const WorkflowDefinition& def, const WorkflowData& input)
    : id_(id), definition_(&def), root_(cloneTemplate(def)), data_(input), outcome_(WorkflowOutcome::RUNNING), started_(false) {
    if (id_.empty()) 
    {
        throw InvalidOperationException("A workflow instance needs an id");
    }
    history_.appendNote("Instance " + id_ + " created from " + def.label());
}

WorkflowInstance::~WorkflowInstance() {}

std::string WorkflowInstance::getDefinitionName() const { return definition_->getName(); }
int WorkflowInstance::getDefinitionVersion() const { return definition_->getVersion(); }

void WorkflowInstance::start() {
    if (started_) 
    {
        throw InvalidOperationException("Instance " + id_ + " has already been started");
    }
    if (isFinished()) 
    {
        throw InvalidOperationException("Instance " + id_ + " is already finished");
    }
    started_ = true;
    root_->start();
    history_.appendNote("Instance " + id_ + " started");
}

void WorkflowInstance::setOutcome(WorkflowOutcome outcome) {
    if (outcome == outcome_) 
    {
        return;
    }
    if (isFinished()) 
    {
        throw InvalidOperationException("Instance " + id_ + " already ended as " + outcomeToString(outcome_) + "; cannot change to " + outcomeToString(outcome));
    }
    outcome_ = outcome;
    history_.appendNote("Instance " + id_ + " outcome: " + outcomeToString(outcome_));
}

void WorkflowInstance::rebindTo(const WorkflowDefinition& def) {
    if (!def.isPublished()) 
    {
        throw InvalidOperationException("Cannot rebind " + id_ + " to unpublished " + def.label());
    }
    if (def.getName() != definition_->getName()) 
    {
        throw InvalidOperationException("Cannot rebind " + id_ + " from '" + definition_->getName() +
                                        "' to a different process '" + def.getName() + "'");
    }
    definition_ = &def;
    history_.appendNote("Instance " + id_ + " rebound to " + def.label());
}

std::unique_ptr<InstanceSnapshot> WorkflowInstance::createSnapshot() const {
    std::unique_ptr<InstanceSnapshot> snap(new InstanceSnapshot());
    snap->instanceId_ = id_;
    snap->data_ = data_;
    snap->outcome_ = outcome_;
    snap->started_ = started_;
    root_->captureState(snap->itemStates_);
    return snap;
}

void WorkflowInstance::restore(const InstanceSnapshot& snapshot) {
    if (snapshot.instanceId_ != id_) 
    {
        throw InvalidOperationException("Snapshot of " + snapshot.instanceId_ +
                                        " cannot be restored into " + id_);
    }
    data_ = snapshot.data_;
    outcome_ = snapshot.outcome_;
    started_ = snapshot.started_;
    root_->restoreState(snapshot.itemStates_);
    history_.appendNote("Instance " + id_ + " restored from checkpoint");
}
