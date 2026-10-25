#include "EscalationLevel.h"
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
    // TODO: Examine the actual EscalationRequest fields, contact role_,
    // track acknowledgement and deadlineSeconds_, then decide whether
    // to handle this level or forward to the next handler.
    // The provided headers do not specify the request interface.
    (void)request;
    throw std::logic_error(
        "EscalationLevel::handle requires EscalationRequest.h and an acknowledgement/deadline API");
}
