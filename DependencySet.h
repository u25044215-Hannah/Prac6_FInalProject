/**
 * @file DependencySet.h
 * @brief Declares the project interfaces and types.
 */

#ifndef DEPENDENCY_SET_H
#define DEPENDENCY_SET_H

#include <string>
#include <vector>
#include "WorkNode.h"

/**
 * @brief Represents the DependencySet component of the workflow engine.
 */
class DependencySet {
private:
    std::vector<std::string> prerequisiteIds_;

public:
    /**
     * @brief Performs the add operation.
     * @param nodeId Parameter used by this operation.
     */
    void add(const std::string& nodeId);
    /**
     * @brief Performs the isSatisfied operation.
     * @param root Parameter used by this operation.
     * @return The result of the operation.
     */

    bool isSatisfied(const WorkNode& root) const;

    /**
     * @brief Checks whether this dependency set contains prerequisites.
     * @return True if at least one prerequisite exists.
     */
    bool hasDependencies() const {
        return !prerequisiteIds_.empty();
    }
};

#endif // DEPENDENCY_SET_H