/**
 * @file RoleAssignment.h
 * @brief Declares the project interfaces and types.
 */

#ifndef ROLE_ASSIGNMENT_H
#define ROLE_ASSIGNMENT_H
#include "AssignmentStrategy.h"


/**
 * @brief Represents the RoleAssignment component of the workflow engine.
 */
class RoleAssignment : public AssignmentStrategy {
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

#endif // ROLE_ASSIGNMENT_H