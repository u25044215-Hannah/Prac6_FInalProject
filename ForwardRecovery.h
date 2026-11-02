/**
 * @file ForwardRecovery.h
 * @brief Declares the project interfaces and types.
 */

#ifndef FORWARD_RECOVERY_H
#define FORWARD_RECOVERY_H

#include "MigrationStrategy.h"


/**
 * @brief Represents the ForwardRecovery component of the workflow engine.
 */
class ForwardRecovery : public MigrationStrategy {
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

#endif // FORWARD_RECOVERY_H