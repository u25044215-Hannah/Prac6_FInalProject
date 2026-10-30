#ifndef TIVIDY_BOTTLENECK_VISITOR_H
#define TIVIDY_BOTTLENECK_VISITOR_H
#include "WorkNodeVisitor.h"
#include <functional>
#include <string>
#include <utility>
#include <vector>
class BottleneckVisitor : public WorkNodeVisitor {
private:
    std::function<bool(const WorkItem&, double&)> durationSeconds;
    std::vector<std::pair<double,std::string>> observed;
public:
    BottleneckVisitor();
    explicit BottleneckVisitor(std::function<bool(const WorkItem&, double&)> source);
    void visit(Stage& s) override;
    void visit(SubProcess& p) override;
    void visit(WorkItem& w) override;
    std::vector<std::string> slowestItems(int n) const;
};
#endif
