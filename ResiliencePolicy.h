#ifndef RESILIENCE_POLICY_H
#define RESILIENCE_POLICY_H

#include "Task.h"
#include "IntegrationTask.h"

class ResiliencePolicy {
public:
    virtual ~ResiliencePolicy() {}
    virtual bool onFailure(IntegrationTask& task, int attempt) = 0;
};

#endif // RESILIENCE_POLICY_H