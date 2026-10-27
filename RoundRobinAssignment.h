#pragma once
#include "AssignmentStrategy.h"
#include "person5Forward.h"

class RoundRobinAssignment : public AssignmentStrategy {
private:
    std::size_t nextIndex_ = 0;
public:
    Participant* selectParticipant(const Task& item,
                                   const std::vector<Participant*>& candidates) override;
};
