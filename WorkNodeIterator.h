#ifndef TIVIDY_WORK_NODE_ITERATOR_H
#define TIVIDY_WORK_NODE_ITERATOR_H

class WorkNode;

// Iterator interface from Figure 5.2.
// Returned WorkNode pointers are non-owning.
class WorkNodeIterator {
public:
    virtual ~WorkNodeIterator() = default;
    virtual bool hasNext() const = 0;
    virtual WorkNode* next() = 0;
};

#endif
