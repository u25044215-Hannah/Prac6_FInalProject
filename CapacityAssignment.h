#ifndef CAPACITY_ASSIGNMENT_H
#define CAPACITY_ASSIGNMENT_H
#include "AssignmentStrategy.h"


class CapacityAssignment : public AssignmentStrategy {
public:
    Participant* selectParticipant(const Task& item,
                                   const std::vector<Participant*>& candidates) override;
};

#endif // CAPACITY_ASSIGNMENT_H