/**
 * @file SequentialRouting.h
 * @brief Declares the project interfaces and types.
 */

#ifndef SEQUENTIAL_ROUTING_H
#define SEQUENTIAL_ROUTING_H
#include "RoutingStrategy.h"


/**
 * @brief Represents the SequentialRouting component of the workflow engine.
 */
class SequentialRouting : public RoutingStrategy {
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
#endif // SEQUENTIAL_ROUTING_H