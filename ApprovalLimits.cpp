#include "ApprovalLimits.h"
#include "ApprovalRequest.h"
#include "WorkflowData.h"
#include <stdexcept>
#include <string>

ApprovalLimits::ApprovalLimits(int maxPriority, const std::set<std::string>& categories)
    : maxPriority_(maxPriority), allowedCategories_(categories) {
    if (maxPriority < 0) throw std::invalid_argument("Maximum priority must be non-negative");
}

bool ApprovalLimits::covers(const ApprovalRequest& request) const {
    // INTEGRATION CONTRACT (not specified by UML): WorkflowData keys are
    // "priority" (nonnegative integer text) and "category" (string).
    // Missing/invalid fields fail closed; check your team's actual data keys.
    const WorkflowData& data = request.getResult();
    if (!data.has("priority") || !data.has("category")) return false;
    const std::string priorityText = data.get("priority");
    int priority = 0;
    try {
        std::size_t consumed = 0;
        priority = std::stoi(priorityText, &consumed);
        if (consumed != priorityText.size() || priority < 0) return false;
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
    const std::string category = data.get("category");
    // Empty allowedCategories_ = no category restriction (integration convention).
    return priority <= maxPriority_ &&
           (allowedCategories_.empty() || allowedCategories_.count(category) != 0);
}
