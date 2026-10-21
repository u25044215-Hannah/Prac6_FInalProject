#ifndef TIVIDY_BOTTLENECK_VISITOR_H
#define TIVIDY_BOTTLENECK_VISITOR_H

#include "WorkNodeVisitor.h"
#include <string>
#include <vector>

// Visitor: identifies the n slowest work items.
class BottleneckVisitor : public WorkNodeVisitor {
public:
    void visit(Stage& s) override;
    void visit(SubProcess& p) override;
    void visit(WorkItem& w) override;
    std::vector<std::string> slowestItems(int n) const;
};

#endif
