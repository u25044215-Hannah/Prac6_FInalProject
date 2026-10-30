#include "TerminalEscalationLevel.h"
#include "EscalationRequest.h"

void TerminalEscalationLevel::handle(EscalationRequest& request) {
    // End of chain. EscalationRequest has no status/resolution-note fields.
    // The final reached level is already stored by the preceding levels.
    (void)request;
}
