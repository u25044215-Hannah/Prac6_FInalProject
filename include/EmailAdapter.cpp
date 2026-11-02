#include "EmailAdapter.h"
#include "SmtpMailService.h"
#include "WorkflowData.h"
#include <stdexcept>
EmailAdapter::EmailAdapter(SmtpMailService* s) : service(s) {}
bool EmailAdapter::isAvailable() const { return service != nullptr; }
WorkflowData EmailAdapter::send(const WorkflowData& request) {
    if (!isAvailable()) throw std::runtime_error("SMTP service not provided");
    const int code = service->sendMail(request.get("to"), request.get("subject"), request.get("html"));
    WorkflowData response;
    response.set("status_code", std::to_string(code));
    return response;
}
