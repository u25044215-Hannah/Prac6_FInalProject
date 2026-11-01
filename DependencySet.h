<<<<<<< Updated upstream
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

=======
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

    /**
     * @brief Checks whether this dependency set contains prerequisites.
     * @return True if at least one prerequisite exists.
     */
    bool hasDependencies() const {
        return !prerequisiteIds_.empty();
    }
};

>>>>>>> Stashed changes
#endif // DEPENDENCY_SET_H