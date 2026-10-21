#pragma once

#include "AssignmentStrategy.h"
#include "Person5Forward.h"

class DirectAssignment : public AssignmentStrategy {
public:
    Participant* selectParticipant(const Task& item,
                                   const std::vector<Participant*>& candidates) override;
};
