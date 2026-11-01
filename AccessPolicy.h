/**
 * @file AccessPolicy.h
 * @brief Declares the project interfaces and types.
 */

#ifndef ACCESSPOLICY_H
#define ACCESSPOLICY_H

#include <map>
#include <set>
#include <string>
#include "Role.h" 


/**
 * @brief Represents the AccessPolicy component of the workflow engine.
 */
class AccessPolicy {
public:
    /**
     * @brief Performs the permit operation.
     * @param roleName Parameter used by this operation.
     * @param operation Parameter used by this operation.
     */
    void permit(const std::string& roleName, const std::string& operation) {
        permitted_[roleName].insert(operation);
    }

    /**
     * @brief Performs the isAllowed operation.
     * @param r Parameter used by this operation.
     * @param operation Parameter used by this operation.
     * @return The result of the operation.
     */
    bool isAllowed(const Role& r, const std::string& operation) const {
        auto it = permitted_.find(r.getName());
        if (it == permitted_.end()) return false;
        return it->second.count(operation) > 0;
    }

private:
    std::map<std::string, std::set<std::string>> permitted_;
};

#endif
