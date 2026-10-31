#include "BottleneckVisitor.h"
#include "Stage.h"
#include "SubProcess.h"
#include "WorkItem.h"
#include <algorithm>
#include <cmath>
#include <utility>
BottleneckVisitor::BottleneckVisitor() = default;
BottleneckVisitor::BottleneckVisitor(
    std::function<bool(const WorkItem&, double&)> source)
    : durationSeconds(std::move(source)) {}
void BottleneckVisitor::visit(Stage&) {}
void BottleneckVisitor::visit(SubProcess&) {}
void BottleneckVisitor::visit(WorkItem& item) {
    if (!durationSeconds) return;
    double seconds = 0.0;
    if (durationSeconds(item, seconds) && std::isfinite(seconds) && seconds >= 0.0)
        observed.emplace_back(seconds, item.getId());
}
std::vector<std::string> BottleneckVisitor::slowestItems(int n) const {
    if (n <= 0) return {};
    auto sorted = observed;
    std::sort(sorted.begin(), sorted.end(), [](const std::pair<double, std::string>& a, const std::pair<double, std::string>& b) {
        return a.first != b.first ? a.first > b.first : a.second < b.second;
    });
    std::vector<std::string> result;
    for (const auto& entry : sorted) {
        if (result.size() >= static_cast<std::size_t>(n)) break;
        result.push_back(entry.second);
    }
    return result;
}
