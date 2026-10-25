#include "CapacityAssignment.h"
#include "Participant.h"
#include <limits>

Participant* CapacityAssignment::selectParticipant(
    const Task& item, const std::vector<Participant*>& candidates) {
    (void)item;
    Participant* best = nullptr;
    int lowestWorkload = std::numeric_limits<int>::max();

    for (Participant* candidate : candidates) {
        if (!candidate) continue;
        const int workload = candidate->getWorkload();
        if (workload < lowestWorkload) {
            lowestWorkload = workload;
            best = candidate;
        }
    }
    return best;
}
