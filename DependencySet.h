#ifndef DEPENDENCY_SET_H
#define DEPENDENCY_SET_H

#include <string>
#include <vector>
#include "WorkNode.h"

class DependencySet {
private:
    std::vector<std::string> prerequisiteIds_;
public:
    void add(const std::string& nodeId);
    bool isSatisfied(const WorkNode& root) const;
};

#endif // DEPENDENCY_SET_H