#ifndef DEPENDENCYRESOLVER_H
#define DEPENDENCYRESOLVER_H

#include "WorkflowEventListener.h"

class WorkflowInstance;

class DependencyResolver : public WorkflowEventListener {
public:
    // The workflow instance must outlive this resolver.
    explicit DependencyResolver(WorkflowInstance& instance);

    // Observer: respond to a published workflow event.
    void onEvent(const WorkflowEvent& event) override;

    // Make tasks with satisfied prerequisites available.
    // Also useful when a workflow first starts.
    void unlockReadyTasks();

private:
    // Non-owning reference to this resolver's workflow instance.
    WorkflowInstance& instance;
};

#endif
