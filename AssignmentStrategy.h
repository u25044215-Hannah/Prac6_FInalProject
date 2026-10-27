#pragma once
#include "Person5Forward.h"

class AssignmentStrategy {
public:
    virtual ~AssignmentStrategy() {}
    virtual Participant* selectParticipant(const Task& item,
                                           const std::vector<Participant*>& candidates) = 0;
};
