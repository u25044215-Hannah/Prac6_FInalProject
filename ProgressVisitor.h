/**
 * @file ProgressVisitor.h
 * @brief Declares the project interfaces and types.
 */

#ifndef TIVIDY_PROGRESS_VISITOR_H
#define TIVIDY_PROGRESS_VISITOR_H
#include "WorkNodeVisitor.h"
#include <cstddef>
/**
 * @brief Represents the ProgressVisitor component of the workflow engine.
 */
class ProgressVisitor : public WorkNodeVisitor {
private:
    std::size_t total = 0;
    std::size_t completed = 0;
public:
    /**
     * @brief Performs the visit operation.
     * @param s Parameter used by this operation.
     */
    void visit(Stage& s) override;
    /**
     * @brief Performs the visit operation.
     * @param p Parameter used by this operation.
     */
    void visit(SubProcess& p) override;
    /**
     * @brief Performs the visit operation.
     * @param w Parameter used by this operation.
     */
    void visit(WorkItem& w) override;
    /**
     * @brief Performs the percentComplete operation.
     * @return The result of the operation.
     */
    double percentComplete() const;
};
#endif
