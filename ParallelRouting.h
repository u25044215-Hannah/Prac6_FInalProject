/**
 * @file ParallelRouting.h
 * @brief Declares the project interfaces and types.
 */

#ifndef PARALLEL_ROUTING_H
#define PARALLEL_ROUTING_H
#include "RoutingStrategy.h"

/**
 * @brief Represents the ParallelRouting component of the workflow engine.
 */
class ParallelRouting : public RoutingStrategy {
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