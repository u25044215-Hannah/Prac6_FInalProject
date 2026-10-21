#ifndef TIVIDY_PROGRESS_VISITOR_H
#define TIVIDY_PROGRESS_VISITOR_H

#include "WorkNodeVisitor.h"

// Visitor: computes percentage of work completed.
class ProgressVisitor : public WorkNodeVisitor {
public:
    void visit(Stage& s) override;
    void visit(SubProcess& p) override;
    void visit(WorkItem& w) override;
    double percentComplete() const;
};

#endif
