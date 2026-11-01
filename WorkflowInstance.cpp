/**
 * @file WorkflowInstance.cpp
 * @brief Implements project functionality.
 */

#include "WorkflowInstance.h"

#include "InstanceSnapshot.h"
#include "TividyException.h"
#include "WorkNode.h"
#include "WorkflowDefinition.h"

// Observer Pattern
#include "Task.h"
#include "DepthFirstIterator.h"
#include "include/EventType.h"

namespace {

// Prototype Pattern:
// Deep-copy the workflow definition's work tree.
std::unique_ptr<WorkNode> cloneTemplate(
    const WorkflowDefinition& def
) {
    std::unique_ptr<WorkNode> copy(
        def.getRoot().clone()
    );

    if (!copy) {
        throw InvalidOperationException(
            "Cloning the template of " +
            def.label() + " failed"
        );
    }

    return copy;
}

}

WorkflowInstance::WorkflowInstance(
    const std::string& id,
    const WorkflowDefinition& def,
    const WorkflowData& input
)
    : id_(id),
      definition_(&def),
      root_(cloneTemplate(def)),
      data_(input),
      outcome_(WorkflowOutcome::RUNNING),
      started_(false) {

    if (id_.empty()) {
        throw InvalidOperationException(
            "A workflow instance needs an id"
        );
    }

    // Observer Pattern:
    // Connect every cloned task to this instance's publisher.
    connectTaskObservers();

    // Create the dependency resolver for this instance.
    dependencyResolver_.reset(
        new DependencyResolver(*this)
    );

    // Subscribe the resolver to task state changes.
    eventPublisher_.subscribe(
        EventType::WORK_ITEM_STATE_CHANGED,
        dependencyResolver_.get()
    );

    history_.appendNote(
        "Instance " + id_ +
        " created from " + def.label()
    );
}

WorkflowInstance::~WorkflowInstance() {
    // Remove the listener before destroying the resolver.
    if (dependencyResolver_) {
        eventPublisher_.unsubscribe(
            EventType::WORK_ITEM_STATE_CHANGED,
            dependencyResolver_.get()
        );
    }
}

std::string WorkflowInstance::getDefinitionName() const {
    return definition_->getName();
}

int WorkflowInstance::getDefinitionVersion() const {
    return definition_->getVersion();
}

void WorkflowInstance::start() {
    if (started_) {
        throw InvalidOperationException(
            "Instance " + id_ +
            " has already been started"
        );
    }

    if (isFinished()) {
        throw InvalidOperationException(
            "Instance " + id_ +
            " is already finished"
        );
    }

    started_ = true;

    root_->start();

    history_.appendNote(
        "Instance " + id_ + " started"
    );
}

void WorkflowInstance::setOutcome(
    WorkflowOutcome outcome
) {
    if (outcome == outcome_) {
        return;
    }

    if (isFinished()) {
        throw InvalidOperationException(
            "Instance " + id_ +
            " already ended as " +
            outcomeToString(outcome_) +
            "; cannot change to " +
            outcomeToString(outcome)
        );
    }

    outcome_ = outcome;

    history_.appendNote(
        "Instance " + id_ +
        " outcome: " +
        outcomeToString(outcome_)
    );
}

void WorkflowInstance::rebindTo(
    const WorkflowDefinition& def
) {
    if (!def.isPublished()) {
        throw InvalidOperationException(
            "Cannot rebind " + id_ +
            " to unpublished " + def.label()
        );
    }

    if (def.getName() != definition_->getName()) {
        throw InvalidOperationException(
            "Cannot rebind " + id_ +
            " from '" + definition_->getName() +
            "' to a different process '" +
            def.getName() + "'"
        );
    }

    definition_ = &def;

    history_.appendNote(
        "Instance " + id_ +
        " rebound to " + def.label()
    );
}

std::unique_ptr<InstanceSnapshot>
WorkflowInstance::createSnapshot() const {

    std::unique_ptr<InstanceSnapshot> snap(
        new InstanceSnapshot()
    );

    snap->instanceId = id_;
    snap->data = data_;
    snap->outcome = outcome_;
    snap->started = started_;

    root_->captureState(snap->itemStates);

    return snap;
}

void WorkflowInstance::restore(
    const InstanceSnapshot& snapshot
) {
    if (snapshot.instanceId != id_) {
        throw InvalidOperationException(
            "Snapshot of " +
            snapshot.instanceId +
            " cannot be restored into " + id_
        );
    }

    data_ = snapshot.data;

    // Bypass setOutcome because rollback may
    // restore an earlier workflow outcome.
    outcome_ = snapshot.outcome;

    started_ = snapshot.started;

    // Task::restoreState() suppresses event publication.
    root_->restoreState(snapshot.itemStates);

    history_.appendNote(
        "Instance " + id_ +
        " restored from checkpoint"
    );
}

void WorkflowInstance::connectTaskObservers() {

    // Traverse the complete cloned workflow tree.
    DepthFirstIterator iterator(root_.get());

    while (iterator.hasNext()) {

        WorkNode* node = iterator.next();

        if (!node) {
            continue;
        }

        // Only concrete tasks need state-change publishers.
        Task* task = dynamic_cast<Task*>(node);

        if (task) {
            task->setEventPublisher(
                &eventPublisher_,
                id_
            );
        }
    }
}