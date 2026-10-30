#ifndef WORKFLOWEVENTLISTENER_H
#define WORKFLOWEVENTLISTENER_H

class WorkflowEvent;

class WorkflowEventListener {
public:
    virtual ~WorkflowEventListener() {}

    virtual void onEvent(const WorkflowEvent& e) = 0;
};

#endif
