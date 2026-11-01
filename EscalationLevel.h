/**
 * @file EscalationLevel.h
 * @brief Declares the project interfaces and types.
 */

#ifndef ESCALATION_LEVEL_H
#define ESCALATION_LEVEL_H

#include "EscalationHandler.h"
#include <string>

/**
 * @brief Represents the EscalationLevel component of the workflow engine.
 */
class EscalationLevel : public EscalationHandler {
private:
    std::string role_;
    int deadlineSeconds_ = 0;
    bool acknowledged_ = false;

public:
    /**
     * @brief Performs the EscalationLevel operation.
     * @param role Parameter used by this operation.
     * @param deadlineSeconds Parameter used by this operation.
     */
    EscalationLevel(const std::string& role, int deadlineSeconds);
    /**
     * @brief Performs the handle operation.
     * @param request Parameter used by this operation.
     */
    void handle(EscalationRequest& request) override;
};

#endif // ESCALATION_LEVEL_H
