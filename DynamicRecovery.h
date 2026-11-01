/**
 * @file DynamicRecovery.h
 * @brief Declares the project interfaces and types.
 */

#ifndef DYNAMIC_MIGRATION_H
#define DYNAMIC_MIGRATION_H
#include "MigrationStrategy.h"


/**
 * @brief Represents the DynamicMigration component of the workflow engine.
 */
class DynamicMigration : public MigrationStrategy {
public:
    /**
     * @brief Performs the migrate operation.
     * @param instance Parameter used by this operation.
     * @param definition Parameter used by this operation.
     * @param recovery Parameter used by this operation.
     */
    void migrate(WorkflowInstance& instance, const WorkflowDefinition& definition,
                 RecoveryManager& recovery) override;
};

#endif // DYNAMIC_MIGRATION_H