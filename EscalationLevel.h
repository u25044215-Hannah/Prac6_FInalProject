#ifndef ESCALATION_LEVEL_H
#define ESCALATION_LEVEL_H

#include "EscalationHandler.h"
#include <string>

class EscalationLevel : public EscalationHandler {
private:
    std::string role_;
    int deadlineSeconds_ = 0;
    bool acknowledged_ = false;

public:
    EscalationLevel(const std::string& role, int deadlineSeconds);
    void handle(EscalationRequest& request) override;
};

#endif // ESCALATION_LEVEL_H
