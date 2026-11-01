/**
 * @file CycleTimeVisitor.h
 * @brief Declares the project interfaces and types.
 */

#ifndef TIVIDY_CYCLE_TIME_VISITOR_H
#define TIVIDY_CYCLE_TIME_VISITOR_H
#include "WorkNodeVisitor.h"
#include <functional>
/**
 * @brief Represents the CycleTimeVisitor component of the workflow engine.
 */
class CycleTimeVisitor : public WorkNodeVisitor {
private:
    std::function<bool(const WorkItem&, double&)> durationSeconds;
    double totalSeconds = 0.0;
    unsigned long count = 0;
public:
    /**
     * @brief Performs the CycleTimeVisitor operation.
     */
    CycleTimeVisitor();
    /**
     * @brief Performs the CycleTimeVisitor operation.
     * @param source Parameter used by this operation.
     * @return The result of the operation.
     */
    explicit CycleTimeVisitor(std::function<bool(const WorkItem&, double&)> source);
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
     * @brief Performs the averageSeconds operation.
     * @return The result of the operation.
     */
    double averageSeconds() const;
};
#endif
