#include "CycleTimeVisitor.h"
#include "Stage.h"
#include "SubProcess.h"
#include "WorkItem.h"
#include <cmath>
#include <utility>
CycleTimeVisitor::CycleTimeVisitor() = default;
CycleTimeVisitor::CycleTimeVisitor(
    std::function<bool(const WorkItem&, double&)> source)
    : durationSeconds(std::move(source)) {}
void CycleTimeVisitor::visit(Stage&) {}
void CycleTimeVisitor::visit(SubProcess&) {}
void CycleTimeVisitor::visit(WorkItem& item) {
    if (!durationSeconds) return;
    double seconds = 0.0;
    if (durationSeconds(item, seconds) && std::isfinite(seconds) && seconds >= 0.0) {
        totalSeconds += seconds;
        ++count;
    }
}
double CycleTimeVisitor::averageSeconds() const {
    return count ? totalSeconds / count : 0.0;
}
