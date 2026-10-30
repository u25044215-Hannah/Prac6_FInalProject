#ifndef EMAILADAPTER_H
#define EMAILADAPTER_H

#include "ExternalConnector.h"

class SmtpMailService;

class EmailAdapter : public ExternalConnector {
private:
    SmtpMailService* service;

public:
    explicit EmailAdapter(SmtpMailService* service);

    WorkflowData send(const WorkflowData& request) override;
    bool isAvailable() const override;
};

#endif
