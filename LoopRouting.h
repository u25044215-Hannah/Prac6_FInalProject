#pragma once
#include "RoutingStrategy.h"
#include "Person5Forward.h"

class LoopRouting : public RoutingStrategy {
private:
    Condition* repeatWhile_ = nullptr; // non-owning until ownership is agreed
    int maxIterations_ = 0;
public:
    std::vector<WorkNode*> nextNodes(WorkNode& done, WorkContainer& container,
                                     const WorkflowData& data) override;
    bool isComplete(const WorkContainer& container) const override;
};
