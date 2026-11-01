/**
 * @file InMemoryDefinitionRepository.h
 * @brief Default DefinitionRepository that keeps everything in memory
 */
#ifndef TIVIDY_IN_MEMORY_DEFINITION_REPOSITORY_H
#define TIVIDY_IN_MEMORY_DEFINITION_REPOSITORY_H

#include "DefinitionRepository.h"
#include <map>

///stores definitions per name, in version order, owned by unique_ptr
class InMemoryDefinitionRepository : public DefinitionRepository {
public:
    /**
     * @brief Performs the InMemoryDefinitionRepository operation.
     */
    InMemoryDefinitionRepository() {}
    /**
     * @brief Performs the InMemoryDefinitionRepository operation.
     */
    virtual ~InMemoryDefinitionRepository() {}
    /**
     * @brief Performs the save operation.
     * @param def Parameter used by this operation.
     * @return The result of the operation.
     */
    virtual const WorkflowDefinition* save(std::unique_ptr<WorkflowDefinition> def);
    /**
     * @brief Performs the find operation.
     * @param name Parameter used by this operation.
     * @param version Parameter used by this operation.
     * @return The result of the operation.
     */
    virtual const WorkflowDefinition* find(const std::string& name, int version) const;
    /**
     * @brief Performs the latest operation.
     * @param name Parameter used by this operation.
     * @return The result of the operation.
     */
    virtual const WorkflowDefinition* latest(const std::string& name) const;
    /**
     * @brief Performs the names operation.
     * @return The result of the operation.
     */
    virtual std::vector<std::string> names() const;
    /**
     * @brief Performs the versionsOf operation.
     * @param name Parameter used by this operation.
     * @return The result of the operation.
     */
    virtual std::vector<int> versionsOf(const std::string& name) const;

private:
    InMemoryDefinitionRepository(const InMemoryDefinitionRepository&);
    InMemoryDefinitionRepository& operator=(const InMemoryDefinitionRepository&);
    typedef std::vector<std::unique_ptr<WorkflowDefinition> > VersionList;
    std::map<std::string, VersionList> store_;
};

#endif // TIVIDY_IN_MEMORY_DEFINITION_REPOSITORY_H