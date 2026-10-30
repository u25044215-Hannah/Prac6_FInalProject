#include "DependecySet.h"
#include <algorithm>
#include <stdexcept>

void DependencySet::add(const std::string& nodeId) {
    if (std::find(prerequisiteIds_.begin(), prerequisiteIds_.end(), nodeId)
        == prerequisiteIds_.end()) prerequisiteIds_.push_back(nodeId);
}
bool DependencySet::isSatisfied(const WorkNode& root) const {
    (void)root;
    if (prerequisiteIds_.empty()) return true;
    // Requires WorkNode.h and an agreed tree traversal to resolve IDs.
    throw std::logic_error("DependencySet::isSatisfied requires WorkNode tree interface");
}
