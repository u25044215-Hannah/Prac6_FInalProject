#include "RetryPolicy.h"
#include <stdexcept>

RetryPolicy::RetryPolicy(int maxAttempts)
    : maxAttempts_(maxAttempts)
{
    if (maxAttempts_ < 1) {
        throw std::invalid_argument("maxAttempts must be at least 1");
    }
}

bool RetryPolicy::onFailure(IntegrationTask& task, int attempt)
{
    (void)task;
    return attempt >= 1 && attempt < maxAttempts_;
}
