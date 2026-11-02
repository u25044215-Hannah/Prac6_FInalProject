/**
 * @file RoundRobinAssignment.h
 * @brief Declares the project interfaces and types.
 */

#ifndef ROUND_ROBIN_ASSIGNMENT_H
#define ROUND_ROBIN_ASSIGNMENT_H

#include "AssignmentStrategy.h"


/**
 * @brief Represents the RoundRobinAssignment component of the workflow engine.
 */
class RoundRobinAssignment : public AssignmentStrategy {
private:
    std::size_t nextIndex_ = 0;
public:
    /**
     * @brief Performs the selectParticipant operation.
     * @param item Parameter used by this operation.
     * @param candidates Parameter used by this operation.
     * @return The result of the operation.
     */
    Participant* selectParticipant(const Task& item,
                                   const std::vector<Participant*>& candidates) override;
};

#endif // ROUND_ROBIN_ASSIGNMENT_H