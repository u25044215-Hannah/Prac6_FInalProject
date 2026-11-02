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
 * Owns every instance. Ids are deterministic so tests are repeatable
 */
class InstanceManager {
public:
    /**
     * @brief Performs the InstanceManager operation.
     */
    InstanceManager() : counter_(0) {}
    /**
     * @brief Performs the InstanceManager operation.
     */
    ~InstanceManager() {}
    ///create (but do not start) an instance
    WorkflowInstance* createInstance(const WorkflowDefinition& def, const WorkflowData& input);
    /**
     * @brief Performs the find operation.
     * @param id Parameter used by this operation.
     * @return The result of the operation.
     */
    WorkflowInstance* find(const std::string& id) const;
    /**
     * @brief Performs the get operation.
     * @param id Parameter used by this operation.
     * @return The result of the operation.
     */
    WorkflowInstance& get(const std::string& id) const;
    ///exact definition version
    std::vector<WorkflowInstance*> runningInstancesOf(const WorkflowDefinition& def) const;
    ///any version of named process
    std::vector<WorkflowInstance*> runningInstancesOf(const std::string& name) const;
    ///all instances, oldest first
    std::vector<WorkflowInstance*> all() const;
    /**
     * @brief Performs the count operation.
     * @return The result of the operation.
     */
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