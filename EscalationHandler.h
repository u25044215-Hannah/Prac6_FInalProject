#ifndef ESCALATION_HANDLER_H
#define ESCALATION_HANDLER_H

#include "EscalationRequest.h"

class EscalationHandler {
protected:
    EscalationHandler* next_ = nullptr; // non-owning unless integrated otherwise
public:
    virtual ~EscalationHandler() {}
    void setNext(EscalationHandler* handler);
    virtual void handle(EscalationRequest& request);
};

#endif // ESCALATION_HANDLER_H