#pragma once
#include "RoutingStrategy.h"
#include "Person5Forward.h"

class ParallelRouting : public RoutingStrategy {
public:
    std::vector<WorkNode*> nextNodes(WorkNode& done, WorkContainer& container,
                                     const WorkflowData& data) override;
    bool isComplete(const WorkContainer& container) const override;
};
