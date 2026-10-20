/**
 * @file InstanceManager.h
 * @brief creates, owns and finds workflow instances
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
 * @brief Creates instances from published definitions (GoF role: Prototype::Client).
 *
 * Instances are created with a unique id, a deep clone of the definition's template tree, 
 * and a copy of the input data. The manager owns all instances and can find them by id or list them.
 * Ids are deterministic ("WF-0001", "WF-0002", ...) for repeatable tests
 * 
 */
class InstanceManager {
public:
    InstanceManager() : counter_(0) {}
    ~InstanceManager() {}

    /**
     * Create (but do not start) an independent instance.
     * @throws InvalidOperationException if @p def is not published
     */
    WorkflowInstance* createInstance(const WorkflowDefinition& def, const WorkflowData& input);

    /// @return the instance, or nullptr if id is unknown
    WorkflowInstance* find(const std::string& id) const;

    /// @return the instance. @throws NotFoundException if the id is unknown
    WorkflowInstance& get(const std::string& id) const;

    ///running instances created from exactly this definition version
    std::vector<WorkflowInstance*> runningInstancesOf(const WorkflowDefinition& def) const;
    /// Running instances of any version of the named process.
    std::vector<WorkflowInstance*> runningInstancesOf(const std::string& name) const;

    ///instances in creation order
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