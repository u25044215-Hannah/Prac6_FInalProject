/**
 * @file TividyEngine.h
 * @brief Single entry point to the generic workflow engine
 */
#ifndef TIVIDY_ENGINE_H
#define TIVIDY_ENGINE_H

#include "CommandProcessor.h"
#include "DefinitionRepository.h"
#include "InstanceManager.h"
#include "RecoveryManager.h"
#include "VersionManager.h"
#include "WorkflowCommand.h"
#include "WorkflowData.h"
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Coordinates definitions, instances, commands, recovery and versioning.
 *
 * The TUI and scenarios use only this class. It holds no industry knowledge:
 * processes arrive as WorkflowDefinition objects built elsewhere
 * 
 */
class TividyEngine {
public:
    ///engine with the default in-memory repository
    TividyEngine();
    ///custom repository
    explicit TividyEngine(std::unique_ptr<DefinitionRepository> repository);
    ~TividyEngine() {}
    ///save, version and publish a definition + apply the version policy
    const WorkflowDefinition* publishDefinition(std::unique_ptr<WorkflowDefinition> def);
    ///create and start an instance of latest version
    WorkflowInstance* startWorkflow(const std::string& name, const WorkflowData& input);
    ///specific published version
    WorkflowInstance* startWorkflow(const std::string& name, int version, const WorkflowData& input);
    ///run a command on an instance and record it in its history (takes ownership)
    void submit(std::unique_ptr<WorkflowCommand> cmd, const std::string& instanceId);
    WorkflowInstance* findInstance(const std::string& id) const { return instances_.find(id); }
    std::vector<WorkflowInstance*> listInstances() const { return instances_.all(); }
    std::vector<std::string> listDefinitionNames() const { return definitions_->names(); }
    const DefinitionRepository& definitions() const { return *definitions_; }
    ///what happens to running instances when a new version is published
    void setMigrationPolicy(std::shared_ptr<MigrationStrategy> policy) { versions_.setPolicy(policy); }
    RecoveryManager& recovery() { return recovery_; }
    InstanceManager& instances() { return instances_; }

private:
    TividyEngine(const TividyEngine&);
    TividyEngine& operator=(const TividyEngine&);
    std::unique_ptr<DefinitionRepository> definitions_;
    InstanceManager instances_;
    RecoveryManager recovery_;
    CommandProcessor commands_;
    VersionManager versions_;
};

#endif // TIVIDY_ENGINE_H