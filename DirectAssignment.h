#ifndef DIRECT_ASSIGNMENT_H
#define DIRECT_ASSIGNMENT_H

#include "AssignmentStrategy.h"

class DirectAssignment : public AssignmentStrategy {
public:
    Participant* selectParticipant(const Task& item,
                                   const std::vector<Participant*>& candidates) override;
};

#endif // DIRECT_ASSIGNMENT_H
