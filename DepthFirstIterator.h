/**
 * @file DepthFirstIterator.h
 * @brief Declares the project interfaces and types.
 */

#ifndef TIVIDY_DEPTH_FIRST_ITERATOR_H
#define TIVIDY_DEPTH_FIRST_ITERATOR_H

#include "WorkNodeIterator.h"
#include <stack>

class WorkNode;

// Concrete iterator: depth-first traversal of the WorkNode tree.
/**
 * @brief Represents the DepthFirstIterator component of the workflow engine.
 */
class DepthFirstIterator : public WorkNodeIterator {
private:
    std::stack<WorkNode*> pending;
    const WorkNode* root = nullptr;

public:
    // Constructor added for practical initialization (not shown in UML).
    /**
     * @brief Performs the DepthFirstIterator operation.
     * @param root Parameter used by this operation.
     * @return The result of the operation.
     */
    explicit DepthFirstIterator(const WorkNode* root);
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
