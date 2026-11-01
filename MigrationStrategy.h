/**
 * @file MigrationStrategy.h
 * @brief Declares the project interfaces and types.
 */

#ifndef MIGRATION_STRATEGY_H
#define MIGRATION_STRATEGY_H
#include "WorkflowInstance.h"
#include "RecoveryManager.h"

/**
 * @brief Represents the MigrationStrategy component of the workflow engine.
 */
class MigrationStrategy {
public:
    /**
     * @brief Performs the MigrationStrategy operation.
     */
    virtual ~MigrationStrategy() {}
    /**
     * @brief Performs the migrate operation.
     * @param instance Parameter used by this operation.
     * @param definition Parameter used by this operation.
     * @param recovery Parameter used by this operation.
     */
    virtual void migrate(WorkflowInstance& instance, const WorkflowDefinition& definition,
                         RecoveryManager& recovery) = 0;
};

#endif // MIGRATION_STRATEGY_H