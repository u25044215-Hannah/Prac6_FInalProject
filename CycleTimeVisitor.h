#ifndef TIVIDY_CYCLE_TIME_VISITOR_H
#define TIVIDY_CYCLE_TIME_VISITOR_H

#include "WorkNodeVisitor.h"

// Visitor: calculates average work-item cycle time in seconds.
class CycleTimeVisitor : public WorkNodeVisitor {
public:
    void visit(Stage& s) override;
    void visit(SubProcess& p) override;
    void visit(WorkItem& w) override;
    double averageSeconds() const;
};

#endif
