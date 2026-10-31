#ifndef ASSIGNMENT_STRATEGY_H
#define ASSIGNMENT_STRATEGY_H

#include <vector>
#include "Participant.h"

class Task;

class AssignmentStrategy {
public:
    virtual ~AssignmentStrategy() {}
    virtual Participant* selectParticipant(const Task& item,
                                           const std::vector<Participant*>& candidates) = 0;
};

#endif // ASSIGNMENT_STRATEGY_H