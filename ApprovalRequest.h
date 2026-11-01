/**
 * @file ApprovalRequest.h
 * @brief Declares the project interfaces and types.
 */

#ifndef APPROVAL_REQUEST_H
#define APPROVAL_REQUEST_H

#include <string>

class WorkflowData;

/**
 * @brief Represents the ApprovalRequest component of the workflow engine.
 */
class ApprovalRequest {
private:
    std::string itemId_;
    WorkflowData* result_ = nullptr; // non-owning; data must outlive request
    std::string decision_;
    std::string reason_;
public:
    /**
     * @brief Performs the ApprovalRequest operation.
     * @param itemId Parameter used by this operation.
     * @param result Parameter used by this operation.
     */
    ApprovalRequest(const std::string& itemId, WorkflowData& result);
    /**
     * @brief Performs the getItemId operation.
     * @return The result of the operation.
     */
    const std::string& getItemId() const;
    /**
     * @brief Performs the getDecision operation.
     * @return The result of the operation.
     */
    const std::string& getDecision() const;
    /**
     * @brief Performs the getReason operation.
     * @return The result of the operation.
     */
    const std::string& getReason() const;
    /**
     * @brief Performs the getResult operation.
     * @return The result of the operation.
     */
    const WorkflowData& getResult() const;
    /**
     * @brief Performs the setDecision operation.
     * @param decision Parameter used by this operation.
     * @param reason Parameter used by this operation.
     */
    void setDecision(const std::string& decision, const std::string& reason);
};

#endif // APPROVAL_REQUEST_H