/**
 * @file WorkflowData.h
 * @brief Declares the project interfaces and types.
 */

#ifndef TIVIDY_WORKFLOW_DATA_H
#define TIVIDY_WORKFLOW_DATA_H

#include <string>
#include <map>
#include <vector>

/**
 * @brief Generic string key/value store.
 * @details Domain-free string key-value pairs for workflow execution
 */
class WorkflowData {
public:
    /**
     * @brief Performs the WorkflowData operation.
     */
    WorkflowData() {}
    /**
     * @brief Performs the get operation.
     * @param key Parameter used by this operation.
     * @return The result of the operation.
     */
    const std::string& get(const std::string& key) const;
    /**
     * @brief Performs the getOr operation.
     * @param key Parameter used by this operation.
     * @param fallback Parameter used by this operation.
     * @return The result of the operation.
     */
    std::string getOr(const std::string& key, const std::string& fallback) const;
    /**
     * @brief Performs the set operation.
     * @param key Parameter used by this operation.
     * @param value Parameter used by this operation.
     */
    void set(const std::string& key, const std::string& value);
    /**
     * @brief Performs the has operation.
     * @param key Parameter used by this operation.
     * @return The result of the operation.
     */
    bool has(const std::string& key) const;
    /**
     * @brief Performs the remove operation.
     * @param key Parameter used by this operation.
     * @return The result of the operation.
     */
    bool remove(const std::string& key);
    /**
     * @brief Performs the keys operation.
     * @return The result of the operation.
     */
    std::vector<std::string> keys() const;
    /**
     * @brief Performs the size operation.
     * @return The result of the operation.
     */
    std::size_t size() const { return values_.size(); }
    /**
     * @brief Performs the empty operation.
     * @return The result of the operation.
     */
    bool empty() const { return values_.empty(); }
    bool operator==(const WorkflowData& other) const { return values_ == other.values_; }
    bool operator!=(const WorkflowData& other) const { return !(*this == other); }

private:
    std::map<std::string, std::string> values_;
};

#endif // TIVIDY_WORKFLOW_DATA_H