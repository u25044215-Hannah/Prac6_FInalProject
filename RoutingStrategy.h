#ifndef ROUTING_STRATEGY_H
#define ROUTING_STRATEGY_H
#include "WorkNode.h"
#include "WorkContainer.h"

class RoutingStrategy {
public:
    virtual ~RoutingStrategy() {}
    virtual std::vector<WorkNode*> nextNodes(WorkNode& done, WorkContainer& container,
                                              const WorkflowData& data) = 0;
    virtual bool isComplete(const WorkContainer& container) const = 0;
};

#endif // ROUTING_STRATEGY_H
