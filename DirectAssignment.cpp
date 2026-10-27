#include "DirectAssignment.h"
#include "Task.h"
#include "Participant.h"

Participant* DirectAssignment::selectParticipant(
    const Task& item, const std::vector<Participant*>& candidates) {
    Participant* chosen = item.getAssignee();
    if (!chosen) return nullptr;

    for (Participant* candidate : candidates) {
        if (candidate == chosen) return chosen;
    }
    return nullptr;
}
