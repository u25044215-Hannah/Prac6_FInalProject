#include "LoopRouting.h"
#include "Condition.h"
#include "WorkContainer.h"
#include "WorkNode.h"
#include "WorkflowData.h"
#include <stdexcept>

std::vector<WorkNode*> LoopRouting::nextNodes(
    WorkNode& done, WorkContainer& container, const WorkflowData& data)
{
    const auto& children = container.getChildren();
    for (std::size_t i = 0; i < children.size(); ++i) {
        if (children[i] != &done) continue;
        // Progress through an iteration sequentially.
        for (std::size_t j = i + 1; j < children.size(); ++j) {
            if (children[j] != nullptr && !children[j]->isComplete())
                return {children[j]};
        }
        // The current iteration reached the end.
        if (repeatWhile != nullptr && repeatWhile->evaluate(data)) {
            // No iteration counter or child reset API appears in the given
            // header, so safe loop repetition cannot be implemented yet.
            throw std::logic_error(
                "LoopRouting needs iteration tracking and child reset before repeating");
        }
        return {};
    }
    return {};
}

bool LoopRouting::isComplete(const WorkContainer& container) const
{
    // This checks the present iteration only: no WorkflowData is supplied.
    for (WorkNode* child : container.getChildren()) {
        if (child != nullptr && !child->isComplete()) return false;
    }
    return true;
}
