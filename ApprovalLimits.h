#ifndef APPROVAL_LIMITS_H
#define APPROVAL_LIMITS_H

#include <set>
#include <string>

class ApprovalRequest;

class ApprovalLimits {
private:
    int maxPriority_ = 0;
    std::set<std::string> allowedCategories_;
public:
    ApprovalLimits(int maxPriority, const std::set<std::string>& categories);
    bool covers(const ApprovalRequest& request) const;
};

#endif // APPROVAL_LIMITS_H
