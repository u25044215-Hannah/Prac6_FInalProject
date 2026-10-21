#pragma once
#include "Person5Forward.h"

class DependencySet {
private:
    std::vector<std::string> prerequisiteIds_;
public:
    void add(const std::string& nodeId);
    bool isSatisfied(const WorkNode& root) const;
};
