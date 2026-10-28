#include "ApprovalLimits.h"
#include "ApprovalRequest.h"
#include <stdexcept>

ApprovalLimits::ApprovalLimits(int maxPriority, const std::set<std::string>& categories)
    : maxPriority_(maxPriority), allowedCategories_(categories) {}

bool ApprovalLimits::covers(const ApprovalRequest& request) const {
    (void)request;
    // Cannot implement this check yet: ApprovalRequest.h exposes neither
    // priority nor category (and provides no public getter for result_).
    // Failing explicitly is safer than approving every request.
    throw std::logic_error("ApprovalLimits::covers requires request priority/category accessors");
}
