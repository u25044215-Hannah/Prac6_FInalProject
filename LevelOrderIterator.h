#ifndef TIVIDY_LEVEL_ORDER_ITERATOR_H
#define TIVIDY_LEVEL_ORDER_ITERATOR_H

#include "WorkNodeIterator.h"
#include <queue>

class WorkNode;

// Concrete iterator: breadth-first / level-order traversal.
class LevelOrderIterator : public WorkNodeIterator {
private:
    std::queue<WorkNode*> pending;

public:
    // Constructor added for practical initialization (not shown in UML).
    explicit LevelOrderIterator(WorkNode* root);
    bool hasNext() const override;
    WorkNode* next() override;
};

#endif
