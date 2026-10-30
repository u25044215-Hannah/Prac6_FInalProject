#ifndef ESCALATION_HANDLER_H
#define ESCALATION_HANDLER_H

class EscalationRequest;

// Chain of Responsibility handler. next_ is non-owning.
class EscalationHandler {
protected:
    EscalationHandler* next_ = nullptr;

public:
    virtual ~EscalationHandler();
    void setNext(EscalationHandler* handler);
    virtual void handle(EscalationRequest& request);
};

#endif // ESCALATION_HANDLER_H
