#include "FallackPolicy.h" 
#include <stdexcept>

FallbackPolicy::FallbackPolicy(const std::string& connector)
    : fallbackConnector_(connector)
{
    if (fallbackConnector_.empty()) {
        throw std::invalid_argument("Fallback connector must not be empty");
    }
}

bool FallbackPolicy::onFailure(IntegrationTask& task, int attempt)
{
    (void)task;
    (void)attempt;
    throw std::logic_error(
        "FallbackPolicy requires IntegrationTask's connector-switching API");
}
