#ifndef TIVIDY_CYCLE_TIME_VISITOR_H
#define TIVIDY_CYCLE_TIME_VISITOR_H
#include "WorkNodeVisitor.h"
#include <functional>
class CycleTimeVisitor : public WorkNodeVisitor {
private:
    std::function<bool(const WorkItem&, double&)> durationSeconds;
    double totalSeconds = 0.0;
    unsigned long count = 0;
public:
    CycleTimeVisitor();
    explicit CycleTimeVisitor(std::function<bool(const WorkItem&, double&)> source);
    void visit(Stage& s) override;
    void visit(SubProcess& p) override;
    void visit(WorkItem& w) override;
    double averageSeconds() const;
};
#endif
