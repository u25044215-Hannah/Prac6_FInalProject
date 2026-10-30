#include "EscalationLevel.h"
#include "EscalationRequest.h"
#include <stdexcept>

EscalationLevel::EscalationLevel(const std::string& role, int deadlineSeconds)
    : role_(role), deadlineSeconds_(deadlineSeconds), acknowledged_(false) {
    if (role_.empty()) {
        throw std::invalid_argument("Escalation role cannot be empty");
    }
    if (deadlineSeconds_ < 0) {
        throw std::invalid_argument("Escalation deadline cannot be negative");
    }
}

void EscalationLevel::handle(EscalationRequest& request) {
    // The shared UML EscalationRequest only exposes item ID and level reached.
    // Record that this level has been visited without accessing non-existent fields.
    request.setLevelReached(request.getLevelReached() + 1);

    // There is no acknowledge operation in the supplied UML, so the flag cannot
    // be modified externally. Deadline scheduling belongs to DeadlineMonitor.
    // This synchronous traversal forwards to the next level when not acknowledged.
    if (!acknowledged_) {
        EscalationHandler::handle(request);
    }
}
