#ifndef CONDITION_H
#define CONDITION_H

#include "WorkflowData.h"
#include <string>

class Condition {
private:
    std::string key_;
    std::string expectedValue_;

public:
    Condition(const std::string& key,
              const std::string& expectedValue)
        : key_(key), expectedValue_(expectedValue) {}

    bool evaluate(const WorkflowData& data) const {
        return data.has(key_) &&
               data.get(key_) == expectedValue_;
    }
};

#endif