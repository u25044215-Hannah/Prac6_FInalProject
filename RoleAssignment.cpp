#include "RoleAssignment.h"
#include "Task.h"
#include "Participant.h"
#include "Role.h"

Participant* RoleAssignment::selectParticipant(
    const Task& item, const std::vector<Participant*>& candidates) {
    // Assumes Task::getRequiredRole() returns const Role* or Role*.
    const Role* required = item.getRequiredRole();
    if (!required) return nullptr;

    for (Participant* candidate : candidates) {
        if (candidate && candidate->hasRole(*required)) return candidate;
    }
    return nullptr;
}
