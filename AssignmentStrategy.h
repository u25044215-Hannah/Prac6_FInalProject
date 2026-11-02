/**
 * @file AssignmentStrategy.h
 * @brief Declares the project interfaces and types.
 */

#ifndef ASSIGNMENT_STRATEGY_H
#define ASSIGNMENT_STRATEGY_H

#include <vector>
#include "Participant.h"

class Task;

/**
 * @brief Represents the AssignmentStrategy component of the workflow engine.
 */
class AssignmentStrategy {
public:
    /**
     * @brief Performs the AssignmentStrategy operation.
     */
    virtual ~AssignmentStrategy() {}
    /**
     * @brief Performs the selectParticipant operation.
     * @param item Parameter used by this operation.
     * @param candidates Parameter used by this operation.
     * @return The result of the operation.
     */
    virtual Participant* selectParticipant(const Task& item,
                                           const std::vector<Participant*>& candidates) = 0;
};

#endif // ASSIGNMENT_STRATEGY_H