#include "WorkflowData.h"
#include "TividyException.h"

const std::string& WorkflowData::get(const std::string& key) const {
    std::map<std::string, std::string>::const_iterator it = values_.find(key);
    if (it == values_.end()) 
    {
        throw NotFoundException("WorkflowData has no key '" + key + "'");
    }
    return it->second;
}

std::string WorkflowData::getOr(const std::string& key, const std::string& fallback) const {
    std::map<std::string, std::string>::const_iterator it = values_.find(key);
    return it == values_.end() ? fallback : it->second;
}

void WorkflowData::set(const std::string& key, const std::string& value) {
    values_[key] = value;
}

bool WorkflowData::has(const std::string& key) const {
    return values_.find(key) != values_.end();
}

bool WorkflowData::remove(const std::string& key) {
    return values_.erase(key) > 0;
}

std::vector<std::string> WorkflowData::keys() const {
    std::vector<std::string> result;
    for (std::map<std::string, std::string>::const_iterator it = values_.begin(); it != values_.end(); ++it) {
        result.push_back(it->first);
    }
    return result;
}