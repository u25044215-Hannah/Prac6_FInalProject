#ifndef ROUND_ROBIN_ASSIGNMENT_H
#define ROUND_ROBIN_ASSIGNMENT_H

#include "AssignmentStrategy.h"


class RoundRobinAssignment : public AssignmentStrategy {
private:
    std::size_t nextIndex_ = 0;
public:
    Participant* selectParticipant(const Task& item,
                                   const std::vector<Participant*>& candidates) override;
};

#endif // ROUND_ROBIN_ASSIGNMENT_H
