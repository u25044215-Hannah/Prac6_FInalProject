#pragma once
#include "Person5Forward.h"

class RoutingStrategy {
public:
    virtual ~RoutingStrategy() {}
    virtual std::vector<WorkNode*> nextNodes(WorkNode& done, WorkContainer& container,
                                              const WorkflowData& data) = 0;
    virtual bool isComplete(const WorkContainer& container) const = 0;
};
