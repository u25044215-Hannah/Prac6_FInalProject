#include "ApprovalLevel.h"
#include "ApprovalRequest.h"
#include <stdexcept>

namespace {
const ApprovalLimits& requireLimits(const ApprovalLimits* limits) {
    if (!limits) throw std::invalid_argument("ApprovalLevel requires limits");
    return *limits;
}
}

ApprovalLevel::ApprovalLevel(const std::string& role, const ApprovalLimits& limits)
    : role_(role), limits_(limits) {
    if (role_.empty()) throw std::invalid_argument("Approval role cannot be empty");
}

ApprovalLevel::ApprovalLevel(const std::string& role, const ApprovalLimits* limits)
    : ApprovalLevel(role, requireLimits(limits)) {}

bool ApprovalLevel::handle(ApprovalRequest& request) {
    if (!request.getDecision().empty()) return request.getDecision() == "approved";
    if (limits_.covers(request)) {
        // INTEGRATION CONVENTION: a covering level approves the request.
        // UML does not specify how to verify the approving actor's role.
        request.setDecision("approved", "Approved at level " + role_);
        return true;
    }
    if (ApprovalHandler::handle(request)) return true;
    request.setDecision("rejected", "No approval level covers this request");
    return false;
}
