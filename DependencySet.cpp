
/**
 * @file DependencySet.cpp
 * @brief Implements project functionality.
 */

#include "DependencySet.h"
#include "WorkNode.h"
#include "WorkItem.h"

#include <algorithm>

void DependencySet::add(const std::string& nodeId) {
    if (std::find(prerequisiteIds_.begin(),
                  prerequisiteIds_.end(),
                  nodeId) == prerequisiteIds_.end()) {

        prerequisiteIds_.push_back(nodeId);
    }
}

bool DependencySet::isSatisfied(const WorkNode& root) const {

    // No prerequisites means the task is ready.
    if (prerequisiteIds_.empty()) {
        return true;
    }

    // WorkNode::findById() is not const, so we use
    // the existing interface without modifying the UML.
    WorkNode& searchableRoot = const_cast<WorkNode&>(root);

    for (const std::string& prerequisiteId : prerequisiteIds_) {

        // Find the prerequisite in the workflow tree.
        WorkNode* node = searchableRoot.findById(prerequisiteId);

        // Missing prerequisite: dependency is not satisfied.
        if (node == nullptr) {
            return false;
        }

        // A prerequisite must be an executable work item.
        WorkItem* item = dynamic_cast<WorkItem*>(node);

        if (item == nullptr) {
            return false;
        }

        // Only successful completion satisfies a dependency.
        if (item->stateName() != "Completed") {
            return false;
        }
    }

    // Every prerequisite has been completed.
    return true;
}
