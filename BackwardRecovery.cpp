
#include "BackwardRecovery.h"
#include "WorkflowInstance.h"
#include "WorkflowDefinition.h"
#include "RecoveryManager.h"

// Save the pre-migration state, then bind to the new definition.
// If rebinding throws, restore the saved state and propagate the error.
// This provides rollback ON FAILURE, not a rewind of completed work.

void BackwardRecovery::migrate(
    WorkflowInstance& instance,
    const WorkflowDefinition& definition,
    RecoveryManager& recovery)
{
    const char* checkpointLabel = "migration_backward";

    // Save the workflow state before migration
    recovery.checkpoint(instance, checkpointLabel);

    try {
        // Rebind to the new workflow definition
        instance.rebindTo(definition);

    } catch (...) {
        // Restore the previous workflow state if migration fails
        recovery.rollback(instance, checkpointLabel);

        // Remove the saved checkpoint
        recovery.discard(instance, checkpointLabel);

        // Propagate the original exception
        throw;
    }

    // Migration succeeded, so remove the checkpoint
    recovery.discard(instance, checkpointLabel);
}
