/**
 * @file CapacityAssignment.h
 * @brief Declares the project interfaces and types.
 */

#ifndef CAPACITY_ASSIGNMENT_H
#define CAPACITY_ASSIGNMENT_H
#include "AssignmentStrategy.h"


/**
 * @brief Represents the CapacityAssignment component of the workflow engine.
 */
class CapacityAssignment : public AssignmentStrategy {
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

#endif // CAPACITY_ASSIGNMENT_H