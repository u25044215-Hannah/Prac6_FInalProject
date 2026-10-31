#ifndef APPROVAL_LEVEL_H
#define APPROVAL_LEVEL_H

#include <string>
#include "ApprovalHandler.h"
#include "ApprovalLimits.h"

class ApprovalLevel : public ApprovalHandler {
private:
    std::string role_;
    ApprovalLimits limits_; // UML shows an ApprovalLimits value, not a pointer
public:
    ApprovalLevel(const std::string& role, const ApprovalLimits& limits);
    // Compatibility overload for existing teammates calling with a pointer.
    ApprovalLevel(const std::string& role, const ApprovalLimits* limits);
    bool handle(ApprovalRequest& request) override;
};

#endif // APPROVAL_LEVEL_H
