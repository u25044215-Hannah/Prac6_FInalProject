/**
 * @file RetryPolicy.h
 * @brief Declares the project interfaces and types.
 */

#ifndef RETRY_POLICY_H
#define RETRY_POLICY_H

#include "ResiliencePolicy.h"


/**
 * @brief Represents the RetryPolicy component of the workflow engine.
 */
class RetryPolicy : public ResiliencePolicy {
private:
    int maxAttempts_ = 0;
public:
    /**
     * @brief Performs the RetryPolicy operation.
     * @param maxAttempts Parameter used by this operation.
     * @return The result of the operation.
     */
    explicit RetryPolicy(int maxAttempts);
    /**
     * @brief Performs the onFailure operation.
     * @param task Parameter used by this operation.
     * @param attempt Parameter used by this operation.
     * @return The result of the operation.
     */
    bool onFailure(IntegrationTask& task, int attempt) override;
};

#endif // RETRY_POLICY_H
