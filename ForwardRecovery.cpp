#include "ForwardRecovery.h"
#include "WorkflowInstance.h"
#include "WorkflowDefinition.h"
#include "RecoveryManager.h"

// Move an existing instance to the supplied definition while preserving
// its current runtime progress through WorkflowInstance::rebindTo().
void ForwardRecovery::migrate(WorkflowInstance& instance,
                              const WorkflowDefinition& definition,
                              RecoveryManager& recovery)
{
    (void)recovery; // Forward migration does not request a rollback.
    instance.rebindTo(&definition);
}
