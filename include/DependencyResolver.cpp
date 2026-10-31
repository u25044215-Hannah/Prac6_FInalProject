#include "DependencyResolver.h"

#include "WorkflowEvent.h"
#include "WorkflowInstance.h"
#include "WorkNode.h"
#include "WorkNodeIterator.h"
#include "DepthFirstIterator.h"
#include "Task.h"

#include <memory>

// Connect this resolver to one specific workflow instance.
DependencyResolver::DependencyResolver(WorkflowInstance& workflow)
    : instance(workflow) {
}


// Observer Pattern:
// Called when the publisher delivers an event.
void DependencyResolver::onEvent(const WorkflowEvent& event) {

    // Only process work-item state changes.
    if (event.getType() != EventType::WORK_ITEM_STATE_CHANGED) {
        return;
    }

    // Ignore events belonging to another workflow instance.
    if (event.getInstanceId() != instance.getId()) {
        return;
    }

    WorkNode& root = instance.getRoot();

    // Find the task that generated the event.
    WorkNode* source = root.findById(event.getSourceId());

    if (source == nullptr) {
        return;
    }

    // Confirm the source is an executable task.
    Task* completedTask = dynamic_cast<Task*>(source);

    if (completedTask == nullptr) {
        return;
    }

    // Only successful completion can unlock dependent tasks.
    if (completedTask->stateName() != "Completed") {
        return;
    }

    // Recheck tasks that may now be eligible.
    unlockReadyTasks();
}


// Check which tasks can become available.
void DependencyResolver::unlockReadyTasks() {

    WorkNode& root = instance.getRoot();

    // Traverse the workflow using the existing Iterator pattern.
    std::unique_ptr<WorkNodeIterator> iterator(
        new DepthFirstIterator(&root)
    );

    while (iterator->hasNext()) {

        WorkNode* node = iterator->next();

        if (node == nullptr) {
            continue;
        }

        // Only executable tasks have task dependencies.
        Task* task = dynamic_cast<Task*>(node);

        if (task == nullptr) {
            continue;
        }

        // Do not interfere with tasks that have already progressed.
        if (task->stateName() != "Created") {
            continue;
        }

        // Check prerequisites against this instance's work tree.
        if (task->getDependencies().isSatisfied(root)) {

            // State Pattern: Created -> Available.
            task->makeAvailable();
        }
    }
}
