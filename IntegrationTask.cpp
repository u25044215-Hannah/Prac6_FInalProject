#include "IntegrationTask.h"
#include <exception>
#include <stdexcept>
#include "ConnectorRegistry.h"
#include "ExternalConnector.h"
#include "ResiliencePolicy.h"

void IntegrationTask::execute() {
    start(); // Assigned -> InProgress; throws IllegalTransitionException otherwise

    while (true) {
        ++attempts_;
        std::string failure;
        try {
            ExternalConnector* connector = connectors_ ? connectors_->find(connectorName_) : nullptr;
            if (!connector) {
                throw std::runtime_error("no connector registered as '" + connectorName_ + "'");
            }
            WorkflowData response = connector->send(WorkflowData());
            state_->complete(*this, response); // InProgress -> Completed
            return;
        } catch (const std::exception& e) {
            failure = e.what();
        }

        // Strategy: the policy decides whether another attempt is worth making.
        // With no policy there is nothing to retry with, so the first failure is final.
        if (policy_ && policy_->onFailure(*this, attempts_)) continue;

        fail("Integration '" + connectorName_ + "' failed after " +
             std::to_string(attempts_) + " attempt(s): " + failure);
        return;
    }
}
