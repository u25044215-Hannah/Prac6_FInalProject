#include "MetricsCollector.h"
#include "WorkflowEvent.h"
#include <iostream>

void MetricsCollector::onEvent(const WorkflowEvent& event) {
    // The existing header has no counters or metrics storage.
    // Emit an event observation for later aggregation by a metrics backend.
    std::clog << "[TIVIDY metric event] type=" << static_cast<int>(event.getType())
              << " instance=" << event.getInstanceId() << '\n';
}
