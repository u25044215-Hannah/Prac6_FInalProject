#ifndef TIVIDY_WORKFLOW_DATA_H
#define TIVIDY_WORKFLOW_DATA_H

#include <string>
#include <map>
#include <vector>

///domain-free string key-value pairs for workflow execution
class WorkflowData {
public:
    WorkflowData() {}
    const std::string& get(const std::string& key) const;   //throws NotFoundException
    std::string getOr(const std::string& key, const std::string& fallback) const;
    void set(const std::string& key, const std::string& value);
    bool has(const std::string& key) const;
    bool remove(const std::string& key);
    std::vector<std::string> keys() const;
    std::size_t size() const { return values_.size(); }
    bool empty() const { return values_.empty(); }
    bool operator==(const WorkflowData& other) const { return values_ == other.values_; }
    bool operator!=(const WorkflowData& other) const { return !(*this == other); }

private:
    std::map<std::string, std::string> values_;
};

#endif // TIVIDY_WORKFLOW_DATA_H