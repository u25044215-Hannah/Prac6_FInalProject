#include "TerminalEscalationLevel.h"
#include <stdexcept>

void TerminalEscalationLevel::handle(EscalationRequest& request)
{
    // Terminal handler: no further escalation available.
    if (request.status != EscalationRequest::Status::Pending) return;
    request.status = EscalationRequest::Status::Unresolved;
    request.resolutionNote = "Unresolved: terminal escalation level reached";
}
