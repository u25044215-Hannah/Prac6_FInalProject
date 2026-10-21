#ifndef TIVIDY_READY_ITEM_ITERATOR_H
#define TIVIDY_READY_ITEM_ITERATOR_H

#include "DepthFirstIterator.h"

class WorkNode;

// Filters depth-first traversal to work items that are ready.
class ReadyItemIterator : public WorkNodeIterator {
private:
    DepthFirstIterator inner;

public:
    // Constructor added for practical initialization (not shown in UML).
    explicit ReadyItemIterator(const WorkNode* root);
    bool hasNext() const override;
    WorkNode* next() override;
};

#endif
