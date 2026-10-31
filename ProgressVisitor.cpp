#include "ProgressVisitor.h"
#include "Stage.h"
#include "SubProcess.h"
#include "WorkItem.h"
void ProgressVisitor::visit(Stage&) {} // containers are not work units
void ProgressVisitor::visit(SubProcess&) {}
void ProgressVisitor::visit(WorkItem& item) {
    ++total;
    if (item.isComplete()) ++completed;
}
double ProgressVisitor::percentComplete() const {
    return total ? 100.0 * completed / total : 0.0;
}
