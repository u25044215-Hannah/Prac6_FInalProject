#include "SmtpMailService.h"
#include <stdexcept>
int SmtpMailService::sendMail(const std::string&, const std::string&, const std::string&) {
    throw std::runtime_error("SMTP transport is not configured; integrate a provider before use");
}
