#ifndef RETRY_POLICY_H
#define RETRY_POLICY_H

#include "ResiliencePolicy.h"


class RetryPolicy : public ResiliencePolicy {
private:
    int maxAttempts_ = 0;
public:
    explicit RetryPolicy(int maxAttempts);
    bool onFailure(IntegrationTask& task, int attempt) override;
};

#endif // RETRY_POLICY_H
