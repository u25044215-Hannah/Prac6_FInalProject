/**
 * @file BottleneckVisitor.h
 * @brief Declares the project interfaces and types.
 */

#ifndef TIVIDY_BOTTLENECK_VISITOR_H
#define TIVIDY_BOTTLENECK_VISITOR_H
#include "WorkNodeVisitor.h"
#include <functional>
#include <string>
#include <utility>
#include <vector>
/**
 * @brief Represents the BottleneckVisitor component of the workflow engine.
 */
class BottleneckVisitor : public WorkNodeVisitor {
private:
    std::function<bool(const WorkItem&, double&)> durationSeconds;
    std::vector<std::pair<double,std::string>> observed;
public:
    /**
     * @brief Performs the BottleneckVisitor operation.
     */
    BottleneckVisitor();
    /**
     * @brief Performs the BottleneckVisitor operation.
     * @param source Parameter used by this operation.
     * @return The result of the operation.
     */
    explicit BottleneckVisitor(std::function<bool(const WorkItem&, double&)> source);
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
     * @brief Performs the slowestItems operation.
     * @param n Parameter used by this operation.
     * @return The result of the operation.
     */
    std::vector<std::string> slowestItems(int n) const;
};
#endif

