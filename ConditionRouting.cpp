#include "ConditionalRouting.h"
#include "Condition.h"
#include "WorkContainer.h"
#include "WorkNode.h"
#include "WorkflowData.h"

std::vector<WorkNode*> ConditionalRouting::nextNodes(
    WorkNode& done, WorkContainer& container, const WorkflowData& data)
{
    (void)done;
    // First matching rule wins. Each pair is (condition, target node ID).
    for (const auto& branch : branches_) {
        if (!branch.first.evaluate(data)) continue;
        for (WorkNode* child : container.getChildren()) {
            if (child != nullptr && child->getId() == branch.second)
                return {child};
        }
        return {}; // Matching rule targets a node outside this container.
    }
    return {}; // No matching rule.
}

bool ConditionalRouting::isComplete(const WorkContainer& container) const
{
    // Conservative interpretation: all children must finish. If only one
    // branch should be required, completion needs chosen-branch state.
    for (WorkNode* child : container.getChildren()) {
        if (child != nullptr && !child->isComplete()) return false;
    }
    return true;
}
