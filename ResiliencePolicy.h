#pragma once
#include "Person5Forward.h"

class ResiliencePolicy {
public:
    virtual ~ResiliencePolicy() {}
    virtual bool onFailure(IntegrationTask& task, int attempt) = 0;
};
