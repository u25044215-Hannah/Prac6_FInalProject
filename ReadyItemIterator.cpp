#include "ReadyItemIterator.h"
#include "WorkItem.h"
#include "WorkNode.h"
#include <stdexcept>
#include <utility>

ReadyItemIterator::ReadyItemIterator(const WorkNode* root)
    : ReadyItemIterator(root, [](const WorkItem& w) {
        // Generic state-only default. Call the injected overload for dependency checks.
        return w.stateName() == "Available";
    }) {}
ReadyItemIterator::ReadyItemIterator(const WorkNode* root,
    std::function<bool(const WorkItem&)> predicate)
    : inner(root), readyPredicate(std::move(predicate)) {
    if (!readyPredicate) throw std::invalid_argument("missing ready predicate");
}
void ReadyItemIterator::fillBuffer() const {
    while (!buffered && inner.hasNext()) {
        WorkNode* n = inner.next();
        auto* item = dynamic_cast<WorkItem*>(n);
        if (item && readyPredicate(*item)) buffered = n;
    }
}
bool ReadyItemIterator::hasNext() const { fillBuffer(); return buffered != nullptr; }
WorkNode* ReadyItemIterator::next() {
    fillBuffer();
    if (!buffered) throw std::out_of_range("ReadyItemIterator exhausted");
    WorkNode* item = buffered; buffered = nullptr;
    return item;
}
