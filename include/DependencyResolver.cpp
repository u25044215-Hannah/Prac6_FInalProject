#include "DependencyResolver.h"
#include "WorkflowEvent.h"
#include <iostream>

void DependencyResolver::onEvent(const WorkflowEvent& event) {
    if (event.getType() != EventType::WORK_ITEM_STATE_CHANGED) return;
    // No task registry or dependency graph is available in this header.
    // Report the signal without inventing or forcing task transitions.
    std::clog << "[TIVIDY dependency signal] instance=" << event.getInstanceId()
              << " source=" << event.getSourceId() << '\n';
}
