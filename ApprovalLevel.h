#pragma once
#include "ApprovalHandler.h"
#include "Person5Forward.h"

class ApprovalLevel : public ApprovalHandler {
private:
    std::string role_;
    ApprovalLimits* limits_ = nullptr; // non-owning configured limits
public:
    ApprovalLevel(const std::string& role, ApprovalLimits* limits);
    bool handle(ApprovalRequest& request) override;
};
