#include "ThirdPartyHttpClient.h"
#include <stdexcept>
std::string ThirdPartyHttpClient::doRequest(const std::string&, const std::string&, const std::string&) {
    throw std::runtime_error("HTTP transport is not configured; integrate an HTTP library before use");
}
