#pragma once
#include "ResiliencePolicy.h"
#include "Person5Forward.h"

class FallbackPolicy : public ResiliencePolicy {
private:
    std::string fallbackConnector_;
public:
    explicit FallbackPolicy(const std::string& connector);
    bool onFailure(IntegrationTask& task, int attempt) override;
};
