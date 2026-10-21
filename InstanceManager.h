/**
 * @file InstanceManager.h
 * @brief Creates, owns and finds workflow instances
 */
#ifndef TIVIDY_INSTANCE_MANAGER_H
#define TIVIDY_INSTANCE_MANAGER_H

#include "WorkflowData.h"
#include "WorkflowInstance.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

class WorkflowDefinition;

/**
 * @brief Creates instances by cloning a definition's template (Prototype client)
 *
 * Owns every instance. Ids are deterministic ("WF-0001", ...) so tests are repeatable
 */
class InstanceManager {
public:
    InstanceManager() : counter_(0) {}
    ~InstanceManager() {}

    ///create (but do not start) an instance
    WorkflowInstance* createInstance(const WorkflowDefinition& def, const WorkflowData& input);

    /// @return the instance, or nullptr if the id is unknown
    WorkflowInstance* find(const std::string& id) const;
    /// @throws NotFoundException if the id is unknown
    WorkflowInstance& get(const std::string& id) const;

    ///running instances of exact definition version
    std::vector<WorkflowInstance*> runningInstancesOf(const WorkflowDefinition& def) const;
    ///running instances of any version of named process
    std::vector<WorkflowInstance*> runningInstancesOf(const std::string& name) const;

    ///all instances, oldest first
    std::vector<WorkflowInstance*> all() const;
    std::size_t count() const { return order_.size(); }

private:
    InstanceManager(const InstanceManager&);
    InstanceManager& operator=(const InstanceManager&);

    std::string nextId();

    std::map<std::string, std::unique_ptr<WorkflowInstance> > instances_;
    std::vector<std::string> order_;
    unsigned long counter_;
};

#endif // TIVIDY_INSTANCE_MANAGER_H