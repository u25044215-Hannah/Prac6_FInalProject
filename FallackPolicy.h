/**
 * @file FallackPolicy.h
 * @brief Declares the project interfaces and types.
 */

#ifndef FALLBACK_POLICY_H
#define FALLBACK_POLICY_H

#include "ResiliencePolicy.h"
#include "Task.h"

/**
 * @brief Represents the FallbackPolicy component of the workflow engine.
 */
class FallbackPolicy : public ResiliencePolicy {
private:
    std::string fallbackConnector_;
public:
    /**
     * @brief Performs the FallbackPolicy operation.
     * @param connector Parameter used by this operation.
     * @return The result of the operation.
     */
    explicit FallbackPolicy(const std::string& connector);
    /**
     * @brief Performs the onFailure operation.
     * @param task Parameter used by this operation.
     * @param attempt Parameter used by this operation.
     * @return The result of the operation.
     */
    bool onFailure(IntegrationTask& task, int attempt) override;
};

#endif // FALLBACK_POLICY_H