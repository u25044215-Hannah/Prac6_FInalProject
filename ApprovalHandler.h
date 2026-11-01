/**
 * @file ApprovalHandler.h
 * @brief Declares the project interfaces and types.
 */

#ifndef APPROVAL_HANDLER_H
#define APPROVAL_HANDLER_H

class ApprovalRequest;

/**
 * @brief Represents the ApprovalHandler component of the workflow engine.
 */
class ApprovalHandler {
protected:
    ApprovalHandler* next_ = nullptr; // non-owning successor
public:
    /**
     * @brief Performs the ApprovalHandler operation.
     */
    virtual ~ApprovalHandler() = default;
    /**
     * @brief Performs the setNext operation.
     * @param handler Parameter used by this operation.
     */
    void setNext(ApprovalHandler* handler);
    /**
     * @brief Performs the handle operation.
     * @param request Parameter used by this operation.
     * @return The result of the operation.
     */
    virtual bool handle(ApprovalRequest& request);
};

#endif // APPROVAL_HANDLER_H
