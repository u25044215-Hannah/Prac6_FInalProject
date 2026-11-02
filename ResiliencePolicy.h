/**
 * @file ResiliencePolicy.h
 * @brief Declares the project interfaces and types.
 */

#ifndef RESILIENCE_POLICY_H
#define RESILIENCE_POLICY_H

#include "Task.h"
class IntegrationTask;
/**
 * @brief Represents the ResiliencePolicy component of the workflow engine.
 */
class ResiliencePolicy {
public:
    /**
     * @brief Performs the ResiliencePolicy operation.
     */
    virtual ~ResiliencePolicy() {}
    /**
     * @brief Performs the onFailure operation.
     * @param task Parameter used by this operation.
     * @param attempt Parameter used by this operation.
     * @return The result of the operation.
     */
    virtual bool onFailure(IntegrationTask& task, int attempt) = 0;
};

#endif // RESILIENCE_POLICY_H