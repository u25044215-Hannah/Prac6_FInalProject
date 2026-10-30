#include "DepthFirstIterator.h"
#include "WorkNode.h"
#include "WorkContainer.h"
#include <stdexcept>

DepthFirstIterator::DepthFirstIterator(const WorkNode* node) : root(node) {
    if (node) pending.push(const_cast<WorkNode*>(node));
}
bool DepthFirstIterator::hasNext() const { return !pending.empty(); }
WorkNode* DepthFirstIterator::next() {
    if (pending.empty()) throw std::out_of_range("DepthFirstIterator exhausted");
    WorkNode* node = pending.top(); pending.pop();
    if (auto* container = dynamic_cast<WorkContainer*>(node)) {
        const auto& children = container->getChildren();
        for (auto it = children.rbegin(); it != children.rend(); ++it)
            if (*it) pending.push(*it);
    }
    return node;
}
