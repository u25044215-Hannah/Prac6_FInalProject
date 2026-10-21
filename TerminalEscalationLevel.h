#pragma once
#include "EscalationHandler.h"
#include "Person5Forward.h"

class TerminalEscalationLevel : public EscalationHandler {
public:
    void handle(EscalationRequest& request) override;
};
