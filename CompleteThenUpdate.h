/**
 * @file CompleteThenUpdate.h
 * @brief Declares the project interfaces and types.
 */

#ifndef COMPLETETHENUPDATE_H
#define COMPLETETHENUPDATE_H

#include "MigrationStrategy.h"

/**
 * @brief Represents the CompleteThenUpdate component of the workflow engine.
 */
class CompleteThenUpdate : public MigrationStrategy {
public:
    /**
     * @brief Performs the migrate operation.
     * @param instance Parameter used by this operation.
     * @param definition Parameter used by this operation.
     * @param recovery Parameter used by this operation.
     */
    void migrate(
        WorkflowInstance& instance,
        const WorkflowDefinition& definition,
        RecoveryManager& recovery
    ) override;
};

#endif