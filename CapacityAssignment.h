#pragma once
#include "AssignmentStrategy.h"
#include "person5Forward.h"

class CapacityAssignment : public AssignmentStrategy {
public:
    Participant* selectParticipant(const Task& item,
                                   const std::vector<Participant*>& candidates) override;
};
