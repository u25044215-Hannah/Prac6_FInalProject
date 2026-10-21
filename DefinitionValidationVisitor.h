#ifndef TIVIDY_DEFINITION_VALIDATION_VISITOR_H
#define TIVIDY_DEFINITION_VALIDATION_VISITOR_H

#include "WorkNodeVisitor.h"
#include <string>
#include <vector>

// Visitor: validates the generic workflow definition.
class DefinitionValidationVisitor : public WorkNodeVisitor {
public:
    std::vector<std::string> errors() const;
    void visit(Stage& s) override;
    void visit(SubProcess& p) override;
    void visit(WorkItem& w) override;
};

#endif
