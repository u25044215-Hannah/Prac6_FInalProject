#pragma once
#include "MigrationStrategy.h"
#include "Person5Forward.h"

class BackwardRecovery : public MigrationStrategy {
public:
    void migrate(WorkflowInstance& instance, const WorkflowDefinition& definition,
                 RecoveryManager& recovery) override;
};
