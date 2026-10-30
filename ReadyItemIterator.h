#ifndef TIVIDY_READY_ITEM_ITERATOR_H
#define TIVIDY_READY_ITEM_ITERATOR_H
#include "DepthFirstIterator.h"
#include <functional>
class WorkItem;
class WorkNode;
// A readiness predicate can also check dependencies from the workflow root.
class ReadyItemIterator : public WorkNodeIterator {
private:
    mutable DepthFirstIterator inner;
    mutable WorkNode* buffered = nullptr;
    std::function<bool(const WorkItem&)> readyPredicate;
    void fillBuffer() const;
public:
    explicit ReadyItemIterator(const WorkNode* root);
    ReadyItemIterator(const WorkNode* root,
        std::function<bool(const WorkItem&)> predicate);
    bool hasNext() const override;
    WorkNode* next() override;
};
#endif
