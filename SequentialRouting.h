#ifndef SEQUENTIAL_ROUTING_H
#define SEQUENTIAL_ROUTING_H
#include "RoutingStrategy.h"


class SequentialRouting : public RoutingStrategy {
public:
    std::vector<WorkNode*> nextNodes(WorkNode& done, WorkContainer& container,
                                     const WorkflowData& data) override;
    bool isComplete(const WorkContainer& container) const override;
};
#endif // SEQUENTIAL_ROUTING_H