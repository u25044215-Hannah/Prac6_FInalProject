#ifndef INTEGRATIONTASK_H
#define INTEGRATIONTASK_H

#include <string>
#include "AutomatedTask.h"

class ResiliencePolicy;   // Strategy (Rules and Decisions)
class ConnectorRegistry;  // Mediator (Events and Surroundings)
class IntegrationTask : public AutomatedTask {
public:
    IntegrationTask(std::string id, std::string name, std::string connectorName,
                    ResiliencePolicy* policy, ConnectorRegistry* connectors,
                    Role* requiredRole = nullptr)
        : AutomatedTask(std::move(id), std::move(name), requiredRole),
          connectorName_(std::move(connectorName)),
          policy_(policy),
          attempts_(0),
          connectors_(connectors) {}

    void execute() override;

    IntegrationTask* clone() const override { return new IntegrationTask(*this); }

    const std::string& getConnectorName() const { return connectorName_; }
    int getAttempts() const { return attempts_; }

private:
    std::string connectorName_;
    ResiliencePolicy* policy_;
    int attempts_;
    ConnectorRegistry* connectors_;
};

#endif
