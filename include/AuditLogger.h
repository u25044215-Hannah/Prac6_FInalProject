#ifndef AUDITLOGGER_H
#define AUDITLOGGER_H

#include "WorkflowEventListener.h"

class AuditLogger : public WorkflowEventListener {
public:
    void onEvent(const WorkflowEvent& e) override;
};

#endif
