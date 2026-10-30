#include "EscalationLevel.h"
#include "EscalationRequest.h"
#include <stdexcept>

EscalationLevel::EscalationLevel(const std::string& role, int deadlineSeconds)
    : role_(role), deadlineSeconds_(deadlineSeconds), acknowledged_(false)
{
    if (role_.empty()) {
        throw std::invalid_argument("Escalation role cannot be empty");
    }
    if (deadlineSeconds_ < 0) {
        throw std::invalid_argument("Escalation deadline cannot be negative");
    }
}

void EscalationLevel::handle(EscalationRequest& request)
{
    if (request.status != EscalationRequest::Status::Pending) return;

    // Record notification once. A real application would dispatch a notification here.
    if (std::find(request.notifiedRoles.begin(), request.notifiedRoles.end(), role_)
            == request.notifiedRoles.end()) {
        request.notifiedRoles.push_back(role_);
    }

    if (request.acknowledgedByRole == role_) {
        request.status = EscalationRequest::Status::Acknowledged;
        request.resolutionNote = "Acknowledged by " + role_;
        return;
    }

    // This standalone model assumes the elapsed time supplied applies to this level.
    if (request.elapsedSeconds >= deadlineSeconds_) {
        if (next_) {
            // A real engine would reset/measure the deadline for the next level.
            // Here each invocation is a simulated evaluation of the whole chain.
            next_->handle(request);
        } else {
            request.status = EscalationRequest::Status::Unresolved;
            request.resolutionNote = "Escalation deadline expired at " + role_;
        }
    }
    // Otherwise remain Pending until another evaluation (e.g. timer event).
}
