/**
 * @file WorkNodeVisitor.h
 * @brief Declares the project interfaces and types.
 */

#ifndef TIVIDY_WORK_NODE_VISITOR_H
#define TIVIDY_WORK_NODE_VISITOR_H

class Stage;
class SubProcess;
class WorkItem;

// Visitor interface: one overload per concrete element kind.
/**
 * @brief Represents the WorkNodeVisitor component of the workflow engine.
 */
class WorkNodeVisitor {
public:
    /**
     * @brief Performs the WorkNodeVisitor operation.
     */
    virtual ~WorkNodeVisitor() = default;
    /**
     * @brief Performs the visit operation.
     * @param s Parameter used by this operation.
     */
    virtual void visit(Stage& s) = 0;
    /**
     * @brief Performs the visit operation.
     * @param p Parameter used by this operation.
     */
    virtual void visit(SubProcess& p) = 0;
    /**
     * @brief Performs the visit operation.
     * @param w Parameter used by this operation.
     */
    virtual void visit(WorkItem& w) = 0;
};

#endif
