#include "InstanceManager.h"
#include "TividyException.h"
#include "WorkflowDefinition.h"
#include <iomanip>
#include <sstream>

std::string InstanceManager::nextId() {
    std::ostringstream out;
    out << "WF-" << std::setw(4) << std::setfill('0') << ++counter_;
    return out.str();
}

WorkflowInstance* InstanceManager::createInstance(const WorkflowDefinition& def, const WorkflowData& input) {
    if (!def.isPublished()) 
    {
        throw InvalidOperationException("Cannot start an instance of unpublished definition '" + def.getName() + "'");
    }
    std::string id = nextId();
    std::unique_ptr<WorkflowInstance> instance(new WorkflowInstance(id, def, input));
    WorkflowInstance* raw = instance.get();
    instances_[id] = std::move(instance);
    order_.push_back(id);
    return raw;
}

WorkflowInstance* InstanceManager::find(const std::string& id) const {
    std::map<std::string, std::unique_ptr<WorkflowInstance> >::const_iterator it = instances_.find(id);
    return it == instances_.end() ? 0 : it->second.get();
}

WorkflowInstance& InstanceManager::get(const std::string& id) const {
    WorkflowInstance* instance = find(id);
    if (!instance) 
    {
        throw NotFoundException("No workflow instance with id '" + id + "'");
    }
    return *instance;
}

std::vector<WorkflowInstance*> InstanceManager::runningInstancesOf(const WorkflowDefinition& def) const {
    std::vector<WorkflowInstance*> result;
    for (std::size_t i = 0; i < order_.size(); ++i) {
        WorkflowInstance* inst = find(order_[i]);
        if (!inst->isFinished() && inst->getDefinitionName() == def.getName() && inst->getDefinitionVersion() == def.getVersion()) 
        {
            result.push_back(inst);
        }
    }
    return result;
}

std::vector<WorkflowInstance*> InstanceManager::runningInstancesOf(const std::string& name) const {
    std::vector<WorkflowInstance*> result;
    for (std::size_t i = 0; i < order_.size(); ++i) {
        WorkflowInstance* inst = find(order_[i]);
        if (!inst->isFinished() && inst->getDefinitionName() == name) 
        {
            result.push_back(inst);
        }
    }
    return result;
}

std::vector<WorkflowInstance*> InstanceManager::all() const {
    std::vector<WorkflowInstance*> result;
    for (std::size_t i = 0; i < order_.size(); ++i) {
        result.push_back(find(order_[i]));
    }
    return result;
}