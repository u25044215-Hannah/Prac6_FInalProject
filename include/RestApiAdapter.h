#ifndef RESTAPIADAPTER_H
#define RESTAPIADAPTER_H

#include "ExternalConnector.h"

class ThirdPartyHttpClient;

class RestApiAdapter : public ExternalConnector {
private:
    ThirdPartyHttpClient* client;

public:
    explicit RestApiAdapter(ThirdPartyHttpClient* client);

    WorkflowData send(const WorkflowData& request) override;
    bool isAvailable() const override;
};

#endif
