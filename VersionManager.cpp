#include "VersionManager.h"
#include "TividyException.h"
#include "WorkflowDefinition.h"
#include <sstream>

const WorkflowDefinition* VersionManager::resolve(const DefinitionRepository& repo, const std::string& name, int version) const {
    const WorkflowDefinition* def = (version == 0) ? repo.latest(name) : repo.find(name, version);
    if (!def) 
    {
        std::ostringstream msg;
        msg << "No published definition '" << name << "'";
        if (version != 0) msg << " version " << version;
        throw NotFoundException(msg.str());
    }
    return def;
}

std::size_t VersionManager::onNewVersion(const WorkflowDefinition& newDef) {
    if (!policy_) 
    {
    return 0;
    }
    std::vector<WorkflowInstance*> running = instances_.runningInstancesOf(newDef.getName());
    std::size_t handled = 0;
    for (std::size_t i = 0; i < running.size(); ++i) {
        if (running[i]->getDefinitionVersion() < newDef.getVersion()) 
        {
            policy_->migrate(*running[i], newDef, recovery_);
            ++handled;
        }
    }
    return handled;
}