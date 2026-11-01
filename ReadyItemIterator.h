/**
 * @file ReadyItemIterator.h
 * @brief Declares the project interfaces and types.
 */

#ifndef TIVIDY_READY_ITEM_ITERATOR_H
#define TIVIDY_READY_ITEM_ITERATOR_H
#include "DepthFirstIterator.h"
#include <functional>
class WorkItem;
class WorkNode;
// A readiness predicate can also check dependencies from the workflow root.
/**
 * @brief Represents the ReadyItemIterator component of the workflow engine.
 */
class ReadyItemIterator : public WorkNodeIterator {
private:
    mutable DepthFirstIterator inner;
    mutable WorkNode* buffered = nullptr;
    std::function<bool(const WorkItem&)> readyPredicate;
    void fillBuffer() const;
public:
    /**
     * @brief Performs the ReadyItemIterator operation.
     * @param root Parameter used by this operation.
     * @return The result of the operation.
     */
    explicit ReadyItemIterator(const WorkNode* root);
    /**
     * @brief Performs the ReadyItemIterator operation.
     * @param root Parameter used by this operation.
     * @param predicate Parameter used by this operation.
     */
    ReadyItemIterator(const WorkNode* root,
        std::function<bool(const WorkItem&)> predicate);
    /**
     * @brief Performs the hasNext operation.
     * @return The result of the operation.
     */
    bool hasNext() const override;
    /**
     * @brief Performs the next operation.
     * @return The result of the operation.
     */
    WorkNode* next() override;
};
#endif
