#ifndef TIVIDY_PROGRESS_VISITOR_H
#define TIVIDY_PROGRESS_VISITOR_H
#include "WorkNodeVisitor.h"
#include <cstddef>
class ProgressVisitor : public WorkNodeVisitor {
private:
    std::size_t total = 0;
    std::size_t completed = 0;
public:
    void visit(Stage& s) override;
    void visit(SubProcess& p) override;
    void visit(WorkItem& w) override;
    double percentComplete() const;
};
#endif
