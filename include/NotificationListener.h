#ifndef NOTIFICATIONLISTENER_H
#define NOTIFICATIONLISTENER_H

#include <string>
#include "WorkflowEventListener.h"

class NotificationListener : public WorkflowEventListener {
private:
    std::string channel;

public:
    explicit NotificationListener(const std::string& channel);
    void onEvent(const WorkflowEvent& e) override;
};

#endif
