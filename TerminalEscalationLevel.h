#ifndef TERMINAL_ESCALATION_LEVEL_H
#define TERMINAL_ESCALATION_LEVEL_H


#include "EscalationHandler.h"


class TerminalEscalationLevel : public EscalationHandler {
public:
    void handle(EscalationRequest& request) override;
};

#endif // TERMINAL_ESCALATION_LEVEL_H
