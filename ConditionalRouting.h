/**
 * @file ConditionalRouting.h
 * @brief Declares the project interfaces and types.
 */

#ifndef CONDITIONAL_ROUTING_H
#define CONDITIONAL_ROUTING_H

#include "RoutingStrategy.h"
#include "Condition.h"
#include <vector>
#include <utility>
#include <string>

/**
 * @brief Represents the ConditionalRouting component of the workflow engine.
 */
class ConditionalRouting : public RoutingStrategy {
private:
    std::vector<std::pair<Condition, std::string> > branches_;
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

#endif // CONDITIONAL_ROUTING_H