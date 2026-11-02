#ifndef THIRDPARTYHTTPCLIENT_H
#define THIRDPARTYHTTPCLIENT_H

#include <string>

class ThirdPartyHttpClient {
public:
    std::string doRequest(const std::string& method,
                          const std::string& url,
                          const std::string& body);
};

#endif
