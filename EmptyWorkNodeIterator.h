/**
 * @file EmptyWorkNodeIterator.h
 * @brief Declares the project interfaces and types.
 */

#ifndef EMPTY_WORK_NODE_ITERATOR_H
#define EMPTY_WORK_NODE_ITERATOR_H

#include "WorkNodeIterator.h"

/**
 * @brief Represents the EmptyWorkNodeIterator component of the workflow engine.
 */
class EmptyWorkNodeIterator : public WorkNodeIterator {
public:
    /**
     * @brief Performs the hasNext operation.
     * @return The result of the operation.
     */
    bool hasNext() const override {
        return false;
    }

    /**
     * @brief Performs the next operation.
     * @return The result of the operation.
     */
    WorkNode* next() override {
        return nullptr;
    }
};

#endif