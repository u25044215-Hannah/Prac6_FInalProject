#ifndef WORKFLOWEVENT_H
#define WORKFLOWEVENT_H

#include <string>
#include "EventType.h"
#include "WorkflowData.h"

class WorkflowEvent {
private:
    EventType type;
    std::string sourceId;
    std::string instanceId;
    WorkflowData payload;

public:
    WorkflowEvent(EventType type,
                  const std::string& sourceId,
                  const std::string& instanceId,
                  const WorkflowData& payload);

    EventType getType() const;
    const std::string& getSourceId() const;
    const std::string& getInstanceId() const;
    const WorkflowData& getPayload() const;
};

#endif
