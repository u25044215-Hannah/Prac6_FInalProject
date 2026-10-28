#include "ParallelRouting.h"
#include "WorkContainer.h"
#include "WorkNode.h"

std::vector<WorkNode*> ParallelRouting::nextNodes(
    WorkNode& done, WorkContainer& container, const WorkflowData& data)
{
    (void)done;
    (void)data;
    std::vector<WorkNode*> result;
    for (WorkNode* child : container.getChildren()) {
        if (child != nullptr && !child->isComplete() && child != &done)
            result.push_back(child);
    }
    return result;
}

bool ParallelRouting::isComplete(const WorkContainer& container) const
{
    for (WorkNode* child : container.getChildren()) {
        if (child != nullptr && !child->isComplete()) return false;
    }
    return true;
}
