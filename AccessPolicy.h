#ifndef ACCESSPOLICY_H
#define ACCESSPOLICY_H

#include <map>
#include <set>
#include <string>
#include "Forward.h"


class AccessPolicy {
public:
    void permit(const std::string& roleName, const std::string& operation) {
        permitted_[roleName].insert(operation);
    }

    bool isAllowed(const Role& r, const std::string& operation) const {
        auto it = permitted_.find(r.name);
        if (it == permitted_.end()) return false;
        return it->second.count(operation) > 0;
    }

private:
    std::map<std::string, std::set<std::string>> permitted_;
};

#endif
