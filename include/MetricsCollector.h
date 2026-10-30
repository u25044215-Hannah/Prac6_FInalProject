#ifndef METRICSCOLLECTOR_H
#define METRICSCOLLECTOR_H

#include "WorkflowEventListener.h"

class MetricsCollector : public WorkflowEventListener {
public:
    void onEvent(const WorkflowEvent& e) override;
};

#endif
