#include "RoundRobinAssignment.h"

Participant* RoundRobinAssignment::selectParticipant(
    const Task& item, const std::vector<Participant*>& candidates) {
    (void)item; // Round-robin is independent of task details.
    if (candidates.empty()) return nullptr;

    // Skip null entries, and continue cycling between calls.
    for (std::size_t tried = 0; tried < candidates.size(); ++tried) {
        const std::size_t index = nextIndex_ % candidates.size();
        nextIndex_ = (index + 1) % candidates.size();
        if (candidates[index] != nullptr) return candidates[index];
    }
    return nullptr;
}
