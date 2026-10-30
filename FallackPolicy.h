#ifndef FALLBACK_POLICY_H
#define FALLBACK_POLICY_H

#include "ResiliencePolicy.h"
#include "Task.h"

class FallbackPolicy : public ResiliencePolicy {
private:
    std::string fallbackConnector_;
public:
    explicit FallbackPolicy(const std::string& connector);
    bool onFailure(IntegrationTask& task, int attempt) override;
};

#endif // FALLBACK_POLICY_H