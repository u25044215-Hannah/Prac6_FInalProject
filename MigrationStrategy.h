#ifndef MIGRATION_STRATEGY_H
#define MIGRATION_STRATEGY_H
#include "WorkflowInstance.h"
#include "RecoveryManager.h"

class MigrationStrategy {
public:
    virtual ~MigrationStrategy() {}
    virtual void migrate(WorkflowInstance& instance, const WorkflowDefinition& definition,
                         RecoveryManager& recovery) = 0;
};

#endif // MIGRATION_STRATEGY_H