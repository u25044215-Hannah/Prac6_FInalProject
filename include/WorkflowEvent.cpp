#include "WorkflowEvent.h"

WorkflowEvent::WorkflowEvent(EventType eventType,
                             const std::string& source,
                             const std::string& instance,
                             const WorkflowData& data)
    : type(eventType), sourceId(source), instanceId(instance), payload(data) {}

EventType WorkflowEvent::getType() const { return type; }
const std::string& WorkflowEvent::getSourceId() const { return sourceId; }
const std::string& WorkflowEvent::getInstanceId() const { return instanceId; }
const WorkflowData& WorkflowEvent::getPayload() const { return payload; }
