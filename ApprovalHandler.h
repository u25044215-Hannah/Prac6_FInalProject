#ifndef APPROVAL_HANDLER_H
#define APPROVAL_HANDLER_H

#include "ApprovalRequest.h"

class ApprovalHandler {
protected:
    ApprovalHandler* next_ = nullptr; // successor; owning policy agreed at integration
public:
    virtual ~ApprovalHandler() {}
    void setNext(ApprovalHandler* handler);
    virtual bool handle(ApprovalRequest& request);
};

#endif // APPROVAL_HANDLER_H
