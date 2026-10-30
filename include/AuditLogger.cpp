#include "AuditLogger.h"
#include "WorkflowEvent.h"
#include <iostream>

void AuditLogger::onEvent(const WorkflowEvent& event) {
    // Basic diagnostic sink; a persistent, configurable audit store is a future integration.
    std::clog << "[TIVIDY event] type=" << static_cast<int>(event.getType())
              << " instance=" << event.getInstanceId()
              << " source=" << event.getSourceId() << '\n';
}
