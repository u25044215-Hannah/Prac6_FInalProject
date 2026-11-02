/**
 * @file EscalationHandler.h
 * @brief Declares the project interfaces and types.
 */

#ifndef ESCALATION_HANDLER_H
#define ESCALATION_HANDLER_H

class EscalationRequest;

// Chain of Responsibility handler. next_ is non-owning.
/**
 * @brief Represents the EscalationHandler component of the workflow engine.
 */
class EscalationHandler {
protected:
    EscalationHandler* next_ = nullptr;

public:
    /**
     * @brief Performs the EscalationHandler operation.
     */
    virtual ~EscalationHandler();
    /**
     * @brief Performs the setNext operation.
     * @param handler Parameter used by this operation.
     */
    void setNext(EscalationHandler* handler);
    /**
     * @brief Performs the handle operation.
     * @param request Parameter used by this operation.
     */
    virtual void handle(EscalationRequest& request);
};

#endif // ESCALATION_HANDLER_H
