#include "SequentialRouting.h"
#include "WorkContainer.h"
#include "WorkNode.h"

std::vector<WorkNode*> SequentialRouting::nextNodes(
    WorkNode& done, WorkContainer& container, const WorkflowData& data)
{
    (void)data;
    const auto& children = container.getChildren();
    for (std::size_t i = 0; i < children.size(); ++i) {
        if (children[i] != &done) continue;
        for (std::size_t j = i + 1; j < children.size(); ++j) {
            if (children[j] != nullptr && !children[j]->isComplete())
                return {children[j]};
        }
        return {};
    }
    return {};
}

bool SequentialRouting::isComplete(const WorkContainer& container) const
{
    for (WorkNode* child : container.getChildren()) {
        if (child != nullptr && !child->isComplete()) return false;
    }
    return true;
}
