/**
 * @file LevelOrderIterator.h
 * @brief Declares the project interfaces and types.
 */

#ifndef TIVIDY_LEVEL_ORDER_ITERATOR_H
#define TIVIDY_LEVEL_ORDER_ITERATOR_H

#include "WorkNodeIterator.h"
#include <queue>

class WorkNode;

// Concrete iterator: breadth-first / level-order traversal.
/**
 * @brief Represents the LevelOrderIterator component of the workflow engine.
 */
class LevelOrderIterator : public WorkNodeIterator {
private:
    std::queue<WorkNode*> pending;

public:
    // Constructor added for practical initialization (not shown in UML).
    /**
     * @brief Performs the LevelOrderIterator operation.
     * @param root Parameter used by this operation.
     * @return The result of the operation.
     */
    explicit LevelOrderIterator(WorkNode* root);
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
