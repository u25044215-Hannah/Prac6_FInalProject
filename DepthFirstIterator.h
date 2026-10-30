#ifndef TIVIDY_DEPTH_FIRST_ITERATOR_H
#define TIVIDY_DEPTH_FIRST_ITERATOR_H

#include "WorkNodeIterator.h"
#include <stack>

class WorkNode;

// Concrete iterator: depth-first traversal of the WorkNode tree.
class DepthFirstIterator : public WorkNodeIterator {
private:
    std::stack<WorkNode*> pending;
    const WorkNode* root = nullptr;

public:
    // Constructor added for practical initialization (not shown in UML).
    explicit DepthFirstIterator(const WorkNode* root);
    bool hasNext() const override;
    WorkNode* next() override;
};

#endif
