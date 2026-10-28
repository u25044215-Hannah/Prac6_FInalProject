#ifndef SMSGATEWAYADAPTER_H
#define SMSGATEWAYADAPTER_H

#include "ExternalConnector.h"

class SmsGatewayService;

class SmsGatewayAdapter : public ExternalConnector {
private:
    SmsGatewayService* service;

public:
    explicit SmsGatewayAdapter(SmsGatewayService* service);

    WorkflowData send(const WorkflowData& request) override;
    bool isAvailable() const override;
};

#endif
