#ifndef APPROVAL_REQUEST_H
#define APPROVAL_REQUEST_H

#include <string>

class WorkflowData;   

class ApprovalRequest {
private:
    std::string itemId_;
    WorkflowData* result_ = nullptr; // non-owning workflow data
    std::string decision_;
    std::string reason_;
public:
    ApprovalRequest(const std::string& itemId, WorkflowData& result);
    const std::string& getItemId() const;
    const std::string& getDecision() const;
    void setDecision(const std::string& decision, const std::string& reason);
};

#endif // APPROVAL_REQUEST_H