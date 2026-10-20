/**
 * @file DefinitionRepository.h
 * @brief Abstract store of published workflow definitions.
 */
#ifndef TIVIDY_DEFINITION_REPOSITORY_H
#define TIVIDY_DEFINITION_REPOSITORY_H

#include "WorkflowDefinition.h"
#include <memory>
#include <string>
#include <vector>

///abstract store of published workflow definitions
class DefinitionRepository {
public:
    virtual ~DefinitionRepository() {}

    /// @return the definition after saving, which is published and has a version.
    virtual const WorkflowDefinition* save(std::unique_ptr<WorkflowDefinition> def) = 0;

    /// @return the definition with @p name and @p version, or nullptr if unknown
    virtual const WorkflowDefinition* find(const std::string& name, int version) const = 0;

    /// @return the latest version of the definition with @p name, or nullptr if unknown
    virtual const WorkflowDefinition* latest(const std::string& name) const = 0;

    /// @return the names of all definitions in the repository (ascending)
    virtual std::vector<std::string> names() const = 0;

    /// @return the versions of all definitions with @p name in the repository (ascending)
    virtual std::vector<int> versionsOf(const std::string& name) const = 0;
};

#endif // TIVIDY_DEFINITION_REPOSITORY_H