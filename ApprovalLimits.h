#pragma once
#include "Person5Forward.h"

class ApprovalLimits {
private:
    int maxPriority_ = 0;
    std::set<std::string> allowedCategories_;
public:
    ApprovalLimits(int maxPriority, const std::set<std::string>& categories);
    bool covers(const ApprovalRequest& request) const;
};
