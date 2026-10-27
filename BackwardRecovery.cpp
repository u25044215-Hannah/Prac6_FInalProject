#include "BackwardRecovery.h"
#include "WorkflowInstance.h"
#include "WorkflowDefinition.h"
#include "RecoveryManager.h"

// Save the pre-migration state, then bind to the new definition.
// If rebinding throws, restore the saved state and propagate the error.
// Note: This provides rollback ON FAILURE, not a rewind of completed work.
void BackwardRecovery::migrate(WorkflowInstance& instance,
                               const WorkflowDefinition& definition,
                               RecoveryManager& recovery)
{
    const char* checkpointLabel = "migration_backward";
    recovery.checkpoint(instance, checkpointLabel);
    try {
        instance.rebindTo(&definition);
    } catch (...) {
        recovery.rollback(instance, checkpointLabel);
        recovery.discard(checkpointLabel);
        throw;
    }
    recovery.discard(checkpointLabel);
}
