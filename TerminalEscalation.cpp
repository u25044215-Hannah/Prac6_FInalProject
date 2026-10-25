#include "TerminalEscalationLevel.h"
#include <stdexcept>

void TerminalEscalationLevel::handle(EscalationRequest& request)
{
    // TODO: Apply the application's terminal escalation action,
    // such as recording an unresolved escalation or notifying an owner.
    // The request's interface and terminal behavior are not supplied.
    (void)request;
    throw std::logic_error(
        "TerminalEscalationLevel::handle requires EscalationRequest.h and a terminal action");
}
