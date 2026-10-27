#pragma once
#include "MigrationStrategy.h"
#include "Person5Forward.h"

class ForwardRecovery : public MigrationStrategy {
public:
    void migrate(WorkflowInstance& instance, const WorkflowDefinition& definition,
                 RecoveryManager& recovery) override;
};
