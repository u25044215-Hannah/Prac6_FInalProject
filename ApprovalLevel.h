#ifndef APPROVAL_LEVEL_H
#define APPROVAL_LEVEL_H
#include "ApprovalHandler.h"

class ApprovalLevel : public ApprovalHandler {
private:
    std::string role_;
    ApprovalLimits* limits_ = nullptr; // non-owning configured limits
public:
    ApprovalLevel(const std::string& role, ApprovalLimits* limits);
    bool handle(ApprovalRequest& request) override;
};

#endif // APPROVAL_LEVEL_H
