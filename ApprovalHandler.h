#ifndef APPROVAL_HANDLER_H
#define APPROVAL_HANDLER_H

class ApprovalRequest;

class ApprovalHandler {
protected:
    ApprovalHandler* next_ = nullptr; // non-owning successor
public:
    virtual ~ApprovalHandler() = default;
    void setNext(ApprovalHandler* handler);
    virtual bool handle(ApprovalRequest& request);
};

#endif // APPROVAL_HANDLER_H
