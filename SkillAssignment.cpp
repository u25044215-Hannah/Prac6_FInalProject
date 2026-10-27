#include "SkillAssignment.h"
#include "Task.h"
#include "Participant.h"
#include <algorithm>
#include <string>
#include <vector>

Participant* SkillAssignment::selectParticipant(
    const Task& item, const std::vector<Participant*>& candidates) {
    // Assumes Task::getRequiredSkills() returns an iterable set of strings.
    // Assumes Participant::getSkills() returns an iterable set of strings.
    const auto& requiredSkills = item.getRequiredSkills();

    Participant* best = nullptr;
    for (Participant* candidate : candidates) {
        if (!candidate) continue;
        const auto& skills = candidate->getSkills();
        const bool allSkillsPresent = std::all_of(
            requiredSkills.begin(), requiredSkills.end(),
            [&](const std::string& required) {
                return std::find(skills.begin(), skills.end(), required) != skills.end();
            });
        if (allSkillsPresent &&
            (!best || candidate->getWorkload() < best->getWorkload())) {
            best = candidate;
        }
    }
    return best;
}
