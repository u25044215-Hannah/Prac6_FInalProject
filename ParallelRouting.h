#ifndef PARALLEL_ROUTING_H
#define PARALLEL_ROUTING_H
#include "RoutingStrategy.h"

class ParallelRouting : public RoutingStrategy {
public:
    std::vector<WorkNode*> nextNodes(WorkNode& done, WorkContainer& container,
                                     const WorkflowData& data) override;
    bool isComplete(const WorkContainer& container) const override;
};

#endif
