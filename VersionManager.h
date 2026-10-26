/**
 * @file VersionManager.h
 * @brief Chooses which definition version to run and what happens to running
 *        instances when a new version is published.
 */
#ifndef TIVIDY_VERSION_MANAGER_H
#define TIVIDY_VERSION_MANAGER_H

#include "DefinitionRepository.h"
#include "InstanceManager.h"
#include "MigrationStrategy.h"
#include "RecoveryManager.h"
#include <memory>
#include <string>

class VersionManager {
public:
    VersionManager(InstanceManager& instances, RecoveryManager& recovery) : instances_(instances), recovery_(recovery) {}
    void setPolicy(std::shared_ptr<MigrationStrategy> policy) { policy_ = policy; }
    MigrationStrategy* getPolicy() const { return policy_.get(); }
    const WorkflowDefinition* resolve(const DefinitionRepository& repo, const std::string& name, int version = 0) const;
    /// @return how many instances were handed over
    std::size_t onNewVersion(const WorkflowDefinition& newDef);

private:
    VersionManager(const VersionManager&);
    VersionManager& operator=(const VersionManager&);
    InstanceManager& instances_;
    RecoveryManager& recovery_;
    std::shared_ptr<MigrationStrategy> policy_;
};

#endif // TIVIDY_VERSION_MANAGER_H