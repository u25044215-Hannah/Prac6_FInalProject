#include "EventTriggerListener.h"
#include "WorkflowEvent.h"
#include <iostream>

void EventTriggerListener::onEvent(const WorkflowEvent& event) {
    // A trigger can be selected using event metadata; this header currently
    // exposes no configuration or workflow-dispatch interface.
    const auto it = triggers.find(event.getSourceId());
    if (it != triggers.end()) {
        std::clog << "[TIVIDY trigger matched] source=" << event.getSourceId()
                  << " configured-action=" << it->second << '\n';
    }
}
