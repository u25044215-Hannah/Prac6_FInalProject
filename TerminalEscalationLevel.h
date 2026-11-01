/**
 * @file TerminalEscalationLevel.h
 * @brief Declares the project interfaces and types.
 */

#ifndef TERMINAL_ESCALATION_LEVEL_H
#define TERMINAL_ESCALATION_LEVEL_H

#include "EscalationHandler.h"

/**
 * @brief Represents the TerminalEscalationLevel component of the workflow engine.
 */
class TerminalEscalationLevel : public EscalationHandler {
public:
    /**
     * @brief Performs the handle operation.
     * @param request Parameter used by this operation.
     */
    void handle(EscalationRequest& request) override;
};

#endif // TERMINAL_ESCALATION_LEVEL_H