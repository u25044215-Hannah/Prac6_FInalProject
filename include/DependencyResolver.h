#ifndef DEPENDENCYRESOLVER_H
#define DEPENDENCYRESOLVER_H

#include "WorkflowEventListener.h"

class DependencyResolver : public WorkflowEventListener {
public:
    void onEvent(const WorkflowEvent& e) override;
};

#endif
