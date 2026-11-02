/**
 * @file IntegrationTask.h
 * @brief Declares the project interfaces and types.
 */

#ifndef INTEGRATIONTASK_H
#define INTEGRATIONTASK_H

#include <string>
#include "AutomatedTask.h"
#include "WorkNode.h"

class ResiliencePolicy;   // Strategy (Rules and Decisions)
class ConnectorRegistry;  // Mediator (Events and Surroundings)
/**
 * @brief Represents the IntegrationTask component of the workflow engine.
 */
class IntegrationTask : public AutomatedTask {
public:
    /**
     * @brief Performs the IntegrationTask operation.
     * @param id Parameter used by this operation.
     * @param name Parameter used by this operation.
     * @param connectorName Parameter used by this operation.
     * @param policy Parameter used by this operation.
     * @param connectors Parameter used by this operation.
     * @param requiredRole Parameter used by this operation.
     */
    IntegrationTask(std::string id, std::string name, std::string connectorName,
                    ResiliencePolicy* policy, ConnectorRegistry* connectors,
                    Role* requiredRole = nullptr)
        : AutomatedTask(std::move(id), std::move(name), requiredRole),
          connectorName_(std::move(connectorName)),
          policy_(policy),
          attempts_(0),
          connectors_(connectors) {}

    // A clone is a new unit of work, so its retry count starts again at zero
    // (the implicit copy would carry the original's attempts over).
    /**
     * @brief Performs the IntegrationTask operation.
     * @param other Parameter used by this operation.
     */
    IntegrationTask(const IntegrationTask& other)
        : AutomatedTask(other),
          connectorName_(other.connectorName_),
          policy_(other.policy_),
          attempts_(0),
          connectors_(other.connectors_) {}

    /**
     * @brief Performs the execute operation.
     */
    void execute() override;

    /**
     * @brief Performs the clone operation.
     * @return The result of the operation.
     */
    IntegrationTask* clone() const override { return new IntegrationTask(*this); }

    /**
     * @brief Performs the getConnectorName operation.
     * @return The result of the operation.
     */
    const std::string& getConnectorName() const { return connectorName_; }
    /**
     * @brief Performs the getAttempts operation.
     * @return The result of the operation.
     */
    int getAttempts() const { return attempts_; }

private:
    std::string connectorName_;
    ResiliencePolicy* policy_;
    int attempts_;
    ConnectorRegistry* connectors_;
};

#endif