#include "LevelOrderIterator.h"
#include "WorkNode.h"
#include "WorkContainer.h"
#include <stdexcept>
LevelOrderIterator::LevelOrderIterator(WorkNode* root) {
    if (root) pending.push(root);
}
bool LevelOrderIterator::hasNext() const { return !pending.empty(); }
WorkNode* LevelOrderIterator::next() {
    if (pending.empty()) throw std::out_of_range("LevelOrderIterator exhausted");
    WorkNode* node = pending.front(); pending.pop();
    if (auto* container = dynamic_cast<WorkContainer*>(node))
        for (WorkNode* child : container->getChildren())
            if (child) pending.push(child);
    return node;
}
