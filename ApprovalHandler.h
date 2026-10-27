#pragma once
#include "Person5Forward.h"

class ApprovalHandler {
protected:
    ApprovalHandler* next_ = nullptr; // successor; owning policy agreed at integration
public:
    virtual ~ApprovalHandler() {}
    void setNext(ApprovalHandler* handler);
    virtual bool handle(ApprovalRequest& request);
};
