#ifndef FORWARD_RECOVERY_H
#define FORWARD_RECOVERY_H

#include "MigrationStrategy.h"


class ForwardRecovery : public MigrationStrategy {
public:
    void migrate(WorkflowInstance& instance, const WorkflowDefinition& definition,
                 RecoveryManager& recovery) override;
};

#endif // FORWARD_RECOVERY_H