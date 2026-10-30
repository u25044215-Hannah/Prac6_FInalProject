#include "NotificationListener.h"
#include "WorkflowEvent.h"
#include <iostream>

NotificationListener::NotificationListener(const std::string& configuredChannel)
    : channel(configuredChannel) {}

void NotificationListener::onEvent(const WorkflowEvent& event) {
    // Diagnostic notification only. Real delivery requires a channel adapter.
    std::clog << "[TIVIDY notification] channel=" << channel
              << " instance=" << event.getInstanceId()
              << " source=" << event.getSourceId()
              << " type=" << static_cast<int>(event.getType()) << '\n';
}
