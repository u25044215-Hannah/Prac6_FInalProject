#include "ApprovalRequest.h"

ApprovalRequest::ApprovalRequest(const std::string& itemId, WorkflowData& result)
    : itemId_(itemId), result_(&result) {}
const std::string& ApprovalRequest::getItemId() const { return itemId_; }
const std::string& ApprovalRequest::getDecision() const { return decision_; }
void ApprovalRequest::setDecision(const std::string& decision, const std::string& reason) {
    decision_ = decision;
    reason_ = reason;
}
