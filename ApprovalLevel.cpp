#include "ApprovalLevel.h"
#include "ApprovalLimits.h"
#include "ApprovalRequest.h"
#include <stdexcept>

ApprovalLevel::ApprovalLevel(const std::string& role, ApprovalLimits* limits)
    : role_(role), limits_(limits)
{
    if (limits_ == nullptr) {
        throw std::invalid_argument("ApprovalLevel requires non-null limits");
    }
}

bool ApprovalLevel::handle(ApprovalRequest& request)
{
    // UML specifies ApprovalLimits::covers(const ApprovalRequest&).
    // A role/actor check cannot be added until the real request API is known.
    if (limits_->covers(request)) {
        return true; // This level can handle the request.
    }
    return ApprovalHandler::handle(request); // Forward to the next level.
}
