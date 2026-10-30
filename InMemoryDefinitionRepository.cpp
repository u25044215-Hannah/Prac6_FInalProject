#include "InMemoryDefinitionRepository.h"
#include "TividyException.h"

const WorkflowDefinition* InMemoryDefinitionRepository::save(std::unique_ptr<WorkflowDefinition> def) {
    if (!def) 
    {
        throw InvalidOperationException("Cannot save a null definition");
    }
    if (def->isPublished()) 
    {
        throw InvalidOperationException("Cannot save " + def->label() + ": it is already published");
    }
    VersionList& versions = store_[def->getName()];
    def->setVersion(static_cast<int>(versions.size()) + 1);
    def->publish();
    const WorkflowDefinition* stored = def.get();
    versions.push_back(std::move(def));
    return stored;
}

const WorkflowDefinition* InMemoryDefinitionRepository::find(const std::string& name, int version) const {
    std::map<std::string, VersionList>::const_iterator it = store_.find(name);
    if (it == store_.end() || version < 1 || version > static_cast<int>(it->second.size())) 
    {
        return 0;
    }
    return it->second[static_cast<std::size_t>(version - 1)].get();
}

const WorkflowDefinition* InMemoryDefinitionRepository::latest(const std::string& name) const {
    std::map<std::string, VersionList>::const_iterator it = store_.find(name);
    if (it == store_.end() || it->second.empty()) 
    {
        return 0;
    }
    return it->second.back().get();
}

std::vector<std::string> InMemoryDefinitionRepository::names() const {
    std::vector<std::string> result;
    for (std::map<std::string, VersionList>::const_iterator it = store_.begin();
         it != store_.end(); ++it) {
        result.push_back(it->first);
    }
    return result;
}

std::vector<int> InMemoryDefinitionRepository::versionsOf(const std::string& name) const {
    std::vector<int> result;
    std::map<std::string, VersionList>::const_iterator it = store_.find(name);
    if (it != store_.end()) 
    {
        for (std::size_t i = 0; i < it->second.size(); ++i) {
            result.push_back(static_cast<int>(i) + 1);
        }
    }
    return result;
}