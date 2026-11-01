/**
 * @file ApprovalLimits.h
 * @brief Declares the project interfaces and types.
 */

#ifndef APPROVAL_LIMITS_H
#define APPROVAL_LIMITS_H

#include <set>
#include <string>

class ApprovalRequest;

/**
 * @brief Represents the ApprovalLimits component of the workflow engine.
 */
class ApprovalLimits {
private:
    int maxPriority_ = 0;
    std::set<std::string> allowedCategories_;
public:
    /**
     * @brief Performs the ApprovalLimits operation.
     * @param maxPriority Parameter used by this operation.
     * @param categories Parameter used by this operation.
     */
    ApprovalLimits(int maxPriority, const std::set<std::string>& categories);
    /**
     * @brief Performs the covers operation.
     * @param request Parameter used by this operation.
     * @return The result of the operation.
     */
    bool covers(const ApprovalRequest& request) const;
};

#endif // APPROVAL_LIMITS_H
