#ifndef TIVIDY_WORK_NODE_VISITOR_H
#define TIVIDY_WORK_NODE_VISITOR_H

class Stage;
class SubProcess;
class WorkItem;

// Visitor interface: one overload per concrete element kind.
class WorkNodeVisitor {
public:
    virtual ~WorkNodeVisitor() = default;
    virtual void visit(Stage& s) = 0;
    virtual void visit(SubProcess& p) = 0;
    virtual void visit(WorkItem& w) = 0;
};

#endif
