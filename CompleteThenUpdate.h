#ifndef COMPLETETHENUPDATE_H
#define COMPLETETHENUPDATE_H

#include "MigrationStrategy.h"

class CompleteThenUpdate : public MigrationStrategy {
public:
    void migrate(
        WorkflowInstance& instance,
        const WorkflowDefinition& definition,
        RecoveryManager& recovery
    ) override;
};

#endif