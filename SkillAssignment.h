#pragma once
#include "AssignmentStrategy.h"
#include "person5Forward.h"

class SkillAssignment : public AssignmentStrategy {
public:
    Participant* selectParticipant(const Task& item,
                                   const std::vector<Participant*>& candidates) override;
};
