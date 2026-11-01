/**
 * @file DefinitionValidationVisitor.h
 * @brief Declares the project interfaces and types.
 */

#ifndef TIVIDY_DEFINITION_VALIDATION_VISITOR_H
#define TIVIDY_DEFINITION_VALIDATION_VISITOR_H
#include "WorkNodeVisitor.h"
#include <functional>
#include <string>
#include <unordered_set>
#include <vector>
/**
 * @brief Represents the DefinitionValidationVisitor component of the workflow engine.
 */
class DefinitionValidationVisitor : public WorkNodeVisitor {
private:
    std::vector<std::string> problems;
    std::unordered_set<std::string> seenIds;
    std::function<std::vector<std::string>(const WorkItem&)> itemRules;
    void validateId(const std::string& id);
public:
    /**
     * @brief Performs the DefinitionValidationVisitor operation.
     */
    DefinitionValidationVisitor();
    /**
     * @brief Performs the DefinitionValidationVisitor operation.
     * @param rules Parameter used by this operation.
     * @return The result of the operation.
     */
    explicit DefinitionValidationVisitor(
        std::function<std::vector<std::string>(const WorkItem&)> rules);
    /**
     * @brief Performs the errors operation.
     * @return The result of the operation.
     */
    std::vector<std::string> errors() const;
    /**
     * @brief Performs the visit operation.
     * @param s Parameter used by this operation.
     */
    void visit(Stage& s) override;
    /**
     * @brief Performs the visit operation.
     * @param p Parameter used by this operation.
     */
    void visit(SubProcess& p) override;
    /**
     * @brief Performs the visit operation.
     * @param w Parameter used by this operation.
     */
    void visit(WorkItem& w) override;
};
#endif
