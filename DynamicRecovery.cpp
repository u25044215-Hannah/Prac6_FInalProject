#include "DynamicRecovery.h"
#include "WorkflowInstance.h"
#include "WorkflowDefinition.h"
#include "RecoveryManager.h"
#include "WorkNode.h"

// Select a migration route based on the live instance: completed workflows
// can rebind directly; active workflows get a checkpoint for safe rollback.
void DynamicMigration::migrate(WorkflowInstance& instance,
                               const WorkflowDefinition& definition,
                               RecoveryManager& recovery)
{
    if (instance.getRoot().isComplete()) {
        instance.rebindTo(definition);
        return;
    }

    const char* checkpointLabel = "migration_dynamic";
    recovery.checkpoint(instance, checkpointLabel);
    try {
        instance.rebindTo(definition);
    } catch (...) {
        recovery.rollback(instance, checkpointLabel);
        recovery.discard(instance, checkpointLabel);;
        throw;
    }
    recovery.discard(instance, checkpointLabel);
}
