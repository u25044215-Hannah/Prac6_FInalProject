#ifndef DYNAMIC_MIGRATION_H
#define DYNAMIC_MIGRATION_H
#include "MigrationStrategy.h"


class DynamicMigration : public MigrationStrategy {
public:
    void migrate(WorkflowInstance& instance, const WorkflowDefinition& definition,
                 RecoveryManager& recovery) override;
};

#endif // DYNAMIC_MIGRATION_H