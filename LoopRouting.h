/**
 * @file LoopRouting.h
 * @brief Declares the project interfaces and types.
 */

#ifndef LOOP_ROUTING_H
#define LOOP_ROUTING_H
#include "Condition.h"
#include "RoutingStrategy.h"
#include "Condition.h"
#include "WorkflowData.h"
#include "WorkContainer.h"
#include "WorkNode.h"

/**
 * @brief Represents the LoopRouting component of the workflow engine.
 */
class LoopRouting : public RoutingStrategy {
private:
    Condition* repeatWhile = nullptr; // non-owning until ownership is agreed
    int maxIterations = 0;
public:
    /**
     * @brief Performs the nextNodes operation.
     * @param done Parameter used by this operation.
     * @param container Parameter used by this operation.
     * @param data Parameter used by this operation.
     * @return The result of the operation.
     */
    std::vector<WorkNode*> nextNodes(WorkNode& done, WorkContainer& container,
                                     const WorkflowData& data) override;
    /**
     * @brief Performs the isComplete operation.
     * @param container Parameter used by this operation.
     * @return The result of the operation.
     */
    bool isComplete(const WorkContainer& container) const override;
};

#endif
