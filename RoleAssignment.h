#ifndef ROLE_ASSIGNMENT_H
#define ROLE_ASSIGNMENT_H
#include "AssignmentStrategy.h"


class RoleAssignment : public AssignmentStrategy {
public:
    Participant* selectParticipant(const Task& item,
                                   const std::vector<Participant*>& candidates) override;
};

#endif // ROLE_ASSIGNMENT_H