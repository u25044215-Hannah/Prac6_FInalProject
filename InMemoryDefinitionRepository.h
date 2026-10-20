/**
 * @file InMemoryDefinitionRepository.h
 * @brief Default DefinitionRepository that keeps everything in memory.
 */
#ifndef TIVIDY_IN_MEMORY_DEFINITION_REPOSITORY_H
#define TIVIDY_IN_MEMORY_DEFINITION_REPOSITORY_H

#include "DefinitionRepository.h"
#include <map>

///stores definitions per name, in version order, owned by unique_ptr.
class InMemoryDefinitionRepository : public DefinitionRepository {
public:
    InMemoryDefinitionRepository() {}
    virtual ~InMemoryDefinitionRepository() {}

    virtual const WorkflowDefinition* save(std::unique_ptr<WorkflowDefinition> def);
    virtual const WorkflowDefinition* find(const std::string& name, int version) const;
    virtual const WorkflowDefinition* latest(const std::string& name) const;
    virtual std::vector<std::string> names() const;
    virtual std::vector<int> versionsOf(const std::string& name) const;

private:
    InMemoryDefinitionRepository(const InMemoryDefinitionRepository&);
    InMemoryDefinitionRepository& operator=(const InMemoryDefinitionRepository&);

    typedef std::vector<std::unique_ptr<WorkflowDefinition> > VersionList;
    std::map<std::string, VersionList> store_;
};

#endif