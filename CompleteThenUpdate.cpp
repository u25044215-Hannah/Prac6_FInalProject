#include "CompleteThenUpdate.h"
#include "WorkflowInstance.h"
#include "WorkflowDefinition.h"
#include "RecoveryManager.h"
#include "WorkNode.h"
#include "WorkNode.h"  // ADD THIS LINE
#include <stdexcept>

// Do not migrate a workflow which still has outstanding work.
void CompleteThenUpdate::migrate(WorkflowInstance& instance,
                                 const WorkflowDefinition& definition,
                                 RecoveryManager& recovery)
{
    (void)recovery;
    if (!instance.getRoot().isComplete()) {
        throw std::logic_error("Cannot migrate until the current workflow is complete");
    }
    
    instance.rebindTo(definition);
    (void)recovery;
}
