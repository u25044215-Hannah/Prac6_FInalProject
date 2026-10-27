#include "SmsGatewayService.h"
#include <stdexcept>
int SmsGatewayService::sendSms(const std::string&, const std::string&) {
    throw std::runtime_error("SMS gateway is not configured; integrate a provider before use");
}
