#ifndef WORKFLOWEVENTPUBLISHER_H
#define WORKFLOWEVENTPUBLISHER_H

#include <map>
#include <vector>
#include "EventType.h"

class WorkflowEvent;
class WorkflowEventListener;

class WorkflowEventPublisher {
private:
    std::map<EventType,
        std::vector<WorkflowEventListener*> > listeners;

public:
    void subscribe(EventType t, WorkflowEventListener* l);
    void unsubscribe(EventType t, WorkflowEventListener* l);
    void publish(const WorkflowEvent& e);
};

#endif
