#include "EscalationHandler.h"
#include <stdexcept>

void EscalationHandler::setNext(EscalationHandler* handler)
{
    if (handler == this) {
        throw std::invalid_argument("An escalation handler cannot follow itself");
    }
    next_ = handler; // Non-owning pointer: caller manages the chain lifetime.
}

void EscalationHandler::handle(EscalationRequest& request)
{
    if (next_ != nullptr) {
        next_->handle(request);
        return;
    }
    throw std::runtime_error("Escalation chain ended without a terminal handler");
}
