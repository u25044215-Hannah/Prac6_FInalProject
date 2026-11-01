/**
 * @file WorkNodeIterator.h
 * @brief Declares the project interfaces and types.
 */

#ifndef TIVIDY_WORK_NODE_ITERATOR_H
#define TIVIDY_WORK_NODE_ITERATOR_H

class WorkNode;

// Returned WorkNode pointers are non-owning.
/**
 * @brief Represents the WorkNodeIterator component of the workflow engine.
 */
class WorkNodeIterator {
public:
    /**
     * @brief Performs the WorkNodeIterator operation.
     */
    virtual ~WorkNodeIterator() = default;
    /**
     * @brief Performs the hasNext operation.
     * @return The result of the operation.
     */
    virtual bool hasNext() const = 0;
    /**
     * @brief Performs the next operation.
     * @return The result of the operation.
     */
    virtual WorkNode* next() = 0;
};

#endif