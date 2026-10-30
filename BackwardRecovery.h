#ifndef BACKWARD_RECOVERY_H
#define BACKWARD_RECOVERY_H
#include "MigrationStrategy.h"


class BackwardRecovery : public MigrationStrategy {
public:
    void migrate(WorkflowInstance& instance, const WorkflowDefinition& definition,
                 RecoveryManager& recovery) override;
};

#endif // BACKWARD_RECOVERY_H