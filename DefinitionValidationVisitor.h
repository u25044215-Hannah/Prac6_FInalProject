#ifndef TIVIDY_DEFINITION_VALIDATION_VISITOR_H
#define TIVIDY_DEFINITION_VALIDATION_VISITOR_H
#include "WorkNodeVisitor.h"
#include <functional>
#include <string>
#include <unordered_set>
#include <vector>
class DefinitionValidationVisitor : public WorkNodeVisitor {
private:
    std::vector<std::string> problems;
    std::unordered_set<std::string> seenIds;
    std::function<std::vector<std::string>(const WorkItem&)> itemRules;
    void validateId(const std::string& id);
public:
    DefinitionValidationVisitor();
    explicit DefinitionValidationVisitor(
        std::function<std::vector<std::string>(const WorkItem&)> rules);
    std::vector<std::string> errors() const;
    void visit(Stage& s) override;
    void visit(SubProcess& p) override;
    void visit(WorkItem& w) override;
};
#endif
