#ifndef SMSGATEWAYSERVICE_H
#define SMSGATEWAYSERVICE_H

#include <string>

class SmsGatewayService {
public:
    int sendSms(const std::string& number,
                const std::string& text);
};

#endif
