#pragma once
#include "ResiliencePolicy.h"
#include "Person5Forward.h"

class RetryPolicy : public ResiliencePolicy {
private:
    int maxAttempts_ = 0;
public:
    explicit RetryPolicy(int maxAttempts);
    bool onFailure(IntegrationTask& task, int attempt) override;
};
