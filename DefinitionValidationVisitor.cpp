#include "DefinitionValidationVisitor.h"
#include "Stage.h"
#include "SubProcess.h"
#include "WorkItem.h"
#include <utility>
DefinitionValidationVisitor::DefinitionValidationVisitor() = default;
DefinitionValidationVisitor::DefinitionValidationVisitor(
    std::function<std::vector<std::string>(const WorkItem&)> rules)
    : itemRules(std::move(rules)) {}
void DefinitionValidationVisitor::validateId(const std::string& id) {
    if (id.empty()) problems.push_back("Work node has empty ID");
    else if (!seenIds.insert(id).second)
        problems.push_back("Duplicate work node ID: " + id);
}
void DefinitionValidationVisitor::visit(Stage& node) { validateId(node.getId()); }
void DefinitionValidationVisitor::visit(SubProcess& node) { validateId(node.getId()); }
void DefinitionValidationVisitor::visit(WorkItem& node) {
    validateId(node.getId());
    if (itemRules) {
        auto extra = itemRules(node);
        problems.insert(problems.end(), extra.begin(), extra.end());
    }
}
std::vector<std::string> DefinitionValidationVisitor::errors() const { return problems; }
