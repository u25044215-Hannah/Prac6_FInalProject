/**
 * @file RoutingStrategy.h
 * @brief Declares the project interfaces and types.
 */

#ifndef ROUTING_STRATEGY_H
#define ROUTING_STRATEGY_H
#include "WorkNode.h"
#include "WorkContainer.h"

/**
 * @brief Represents the RoutingStrategy component of the workflow engine.
 */
class RoutingStrategy {
public:
    /**
     * @brief Performs the RoutingStrategy operation.
     */
    virtual ~RoutingStrategy() {}
    /**
     * @brief Performs the nextNodes operation.
     * @param done Parameter used by this operation.
     * @param container Parameter used by this operation.
     * @param data Parameter used by this operation.
     * @return The result of the operation.
     */
    virtual std::vector<WorkNode*> nextNodes(WorkNode& done, WorkContainer& container,
                                              const WorkflowData& data) = 0;
    /**
     * @brief Performs the isComplete operation.
     * @param container Parameter used by this operation.
     * @return The result of the operation.
     */
    virtual bool isComplete(const WorkContainer& container) const = 0;
};

#endif // ROUTING_STRATEGY_H
