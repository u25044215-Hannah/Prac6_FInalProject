#ifndef SKILL_ASSIGNMENT_H
#define SKILL_ASSIGNMENT_H
#include "AssignmentStrategy.h"


class SkillAssignment : public AssignmentStrategy {
public:
    Participant* selectParticipant(const Task& item,
                                   const std::vector<Participant*>& candidates) override;
};
#endif // SKILL_ASSIGNMENT_H
