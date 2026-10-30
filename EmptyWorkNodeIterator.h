#ifndef EMPTY_WORK_NODE_ITERATOR_H
#define EMPTY_WORK_NODE_ITERATOR_H

#include "WorkNodeIterator.h"

class EmptyWorkNodeIterator : public WorkNodeIterator {
public:
    bool hasNext() const override {
        return false;
    }

    WorkNode* next() override {
        return nullptr;
    }
};

#endif