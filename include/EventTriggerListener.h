#ifndef EVENTTRIGGERLISTENER_H
#define EVENTTRIGGERLISTENER_H

#include <map>
#include <string>
#include "WorkflowEventListener.h"

class EventTriggerListener : public WorkflowEventListener {
private:
    std::map<std::string, std::string> triggers;

public:
    void onEvent(const WorkflowEvent& e) override;
};

#endif
