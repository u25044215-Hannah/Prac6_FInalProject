#include "RestApiAdapter.h"
#include "ThirdPartyHttpClient.h"
#include "WorkflowData.h"
#include <stdexcept>
RestApiAdapter::RestApiAdapter(ThirdPartyHttpClient* c) : client(c) {}
bool RestApiAdapter::isAvailable() const { return client != nullptr; }
WorkflowData RestApiAdapter::send(const WorkflowData& request) {
    if (!isAvailable()) throw std::runtime_error("HTTP client not provided");
    const std::string body = client->doRequest(request.get("method"), request.get("url"), request.getOr("body", ""));
    WorkflowData response;
    response.set("body", body);
    return response;
}
