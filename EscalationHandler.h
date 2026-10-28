#pragma once
#include "Person5Forward.h"

class EscalationHandler {
protected:
    EscalationHandler* next_ = nullptr; // non-owning unless integrated otherwise
public:
    virtual ~EscalationHandler() {}
    void setNext(EscalationHandler* handler);
    virtual void handle(EscalationRequest& request);
};
