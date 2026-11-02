#include "SmsGatewayAdapter.h"
#include "SmsGatewayService.h"
#include "WorkflowData.h"
#include <stdexcept>
SmsGatewayAdapter::SmsGatewayAdapter(SmsGatewayService* s) : service(s) {}
bool SmsGatewayAdapter::isAvailable() const { return service != nullptr; }
WorkflowData SmsGatewayAdapter::send(const WorkflowData& request) {
    if (!isAvailable()) throw std::runtime_error("SMS service not provided");
    const int code = service->sendSms(request.get("number"), request.get("text"));
    WorkflowData response;
    response.set("status_code", std::to_string(code));
    return response;
}
