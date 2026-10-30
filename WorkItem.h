#ifndef WORKITEM_H
#define WORKITEM_H

#include <string>
#include <utility>
#include "WorkflowData.h"
#include "WorkNode.h"
#include "Participant.h"

class WorkItem : public WorkNode {
public:
    WorkItem(std::string id, std::string name) : WorkNode(std::move(id), std::move(name)) {}
    ~WorkItem() override = default;

    virtual void makeAvailable() = 0;
    virtual void assign(Participant& p, Participant& actor) = 0;
    virtual void acceptAssignment(Participant& actor) = 0;
    virtual void declineAssignment(Participant& actor) = 0;
    virtual void complete(WorkflowData result, Participant& actor) = 0;
    virtual void fail(const std::string& reason) = 0;
    virtual void reject(const std::string& reason, Participant& actor) = 0;
    virtual void escalate() = 0;
    virtual void hold() = 0;
    virtual void release() = 0;
    virtual std::string stateName() const = 0;
};

#endif // WORKITEM_H