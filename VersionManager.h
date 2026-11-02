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

/**
 * @brief Represents the VersionManager component of the workflow engine.
 */
class VersionManager {
public:
    /**
     * @brief Performs the VersionManager operation.
     * @param instances Parameter used by this operation.
     * @param recovery Parameter used by this operation.
     */
    VersionManager(InstanceManager& instances, RecoveryManager& recovery) : instances_(instances), recovery_(recovery) {}
    /**
     * @brief Performs the setPolicy operation.
     * @param policy Parameter used by this operation.
     */
    void setPolicy(std::shared_ptr<MigrationStrategy> policy) { policy_ = policy; }
    /**
     * @brief Performs the getPolicy operation.
     * @return The result of the operation.
     */
    MigrationStrategy* getPolicy() const { return policy_.get(); }
    /**
     * @brief Performs the resolve operation.
     * @param repo Parameter used by this operation.
     * @param name Parameter used by this operation.
     * @param version Parameter used by this operation.
     * @return The result of the operation.
     */
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