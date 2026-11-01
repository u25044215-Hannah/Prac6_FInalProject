/**
 * @file Condition.h
 * @brief Declares the project interfaces and types.
 */

#ifndef CONDITION_H
#define CONDITION_H

#include "WorkflowData.h"
#include <string>

/**
 * @brief Represents the Condition component of the workflow engine.
 */
class Condition {
private:
    std::string key_;
    std::string expectedValue_;

public:
    /**
     * @brief Performs the Condition operation.
     * @param key Parameter used by this operation.
     * @param expectedValue Parameter used by this operation.
     */
    Condition(const std::string& key,
              const std::string& expectedValue)
        : key_(key), expectedValue_(expectedValue) {}

    /**
     * @brief Performs the evaluate operation.
     * @param data Parameter used by this operation.
     * @return The result of the operation.
     */
    bool evaluate(const WorkflowData& data) const {
        return data.has(key_) &&
               data.get(key_) == expectedValue_;
    }
};

#endif