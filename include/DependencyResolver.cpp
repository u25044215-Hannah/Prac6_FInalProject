#include "DependencyResolver.h"

#include "WorkflowInstance.h"
#include "WorkflowEvent.h"
#include "WorkNode.h"
#include "Task.h"
#include "DepthFirstIterator.h"

// Constructor
DependencyResolver::DependencyResolver(WorkflowInstance& instance)
    : instance(instance)
{
}

// Observer Pattern:
// React when a task changes state.
void DependencyResolver::onEvent(const WorkflowEvent& event)
{
    // Only process task state-change events.
    if (event.getType() != EventType::WORK_ITEM_STATE_CHANGED)
    {
        return;
    }

    // Ignore events belonging to other workflow instances.
    if (event.getInstanceId() != instance.getId())
    {
        return;
    }

    WorkNode& root = instance.getRoot();

    // Find the task that published the event.
    WorkNode* source = root.findById(event.getSourceId());

    if (!source)
    {
        return;
    }

    Task* completedTask = dynamic_cast<Task*>(source);

    if (!completedTask)
    {
        return;
    }

    // Only unlock dependent tasks when a prerequisite
    // has successfully completed.
    if (completedTask->stateName() != "Completed")
    {
        return;
    }

    // Check whether any dependent tasks can become available.
    unlockReadyTasks();
}

// Dependency Resolution:
// Unlock tasks whose prerequisites have all been completed.
void DependencyResolver::unlockReadyTasks()
{
    WorkNode& root = instance.getRoot();

    // Traverse the entire workflow tree.
    DepthFirstIterator iterator(&root);

    while (iterator.hasNext())
    {
        WorkNode* node = iterator.next();

        if (!node)
        {
            continue;
        }

        Task* task = dynamic_cast<Task*>(node);

        if (!task)
        {
            continue;
        }

        // Only consider tasks that have not yet become available.
        if (task->stateName() != "Created")
        {
            continue;
        }

        // IMPORTANT:
        // Tasks without explicit dependencies must be controlled
        // by normal workflow routing, not the dependency resolver.
        if (!task->getDependencies().hasDependencies())
        {
            continue;
        }

        // Unlock the task only when ALL prerequisites
        // have successfully completed.
        if (task->getDependencies().isSatisfied(root))
        {
            task->makeAvailable();
        }
    }
}