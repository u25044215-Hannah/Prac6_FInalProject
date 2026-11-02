/**
 * @file BackwardRecovery.h
 * @brief Declares the project interfaces and types.
 */

#ifndef BACKWARD_RECOVERY_H
#define BACKWARD_RECOVERY_H
#include "MigrationStrategy.h"


/**
 * @brief Represents the BackwardRecovery component of the workflow engine.
 */
class BackwardRecovery : public MigrationStrategy {
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

#endif // BACKWARD_RECOVERY_H