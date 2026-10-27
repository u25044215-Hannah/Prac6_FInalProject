#pragma once
#include "EscalationHandler.h"
#include "Person5Forward.h"

class EscalationLevel : public EscalationHandler {
private:
    std::string role_;
    int deadlineSeconds_ = 0;
    bool acknowledged_ = false;
public:
    EscalationLevel(const std::string& role, int deadlineSeconds);
    void handle(EscalationRequest& request) override;
};
