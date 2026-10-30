#ifndef LOOP_ROUTING_H
#define LOOP_ROUTING_H
#include "RoutingStrategy.h"
#include "Condition.h"
#include "WorkflowData.h"
#include "WorkContainer.h"
#include "WorkNode.h"

class LoopRouting : public RoutingStrategy {
private:
    Condition* repeatWhile = nullptr; // non-owning until ownership is agreed
    int maxIterations = 0;
public:
    std::vector<WorkNode*> nextNodes(WorkNode& done, WorkContainer& container,
                                     const WorkflowData& data) override;
    bool isComplete(const WorkContainer& container) const override;
};

#endif
