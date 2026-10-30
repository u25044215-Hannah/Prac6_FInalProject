#ifndef SUBPROCESS_H
#define SUBPROCESS_H

#include "WorkContainer.h"
#include "WorkflowData.h"      // Person 1
#include "WorkflowOutcome.h"   // Person 1

class SubProcess : public WorkContainer {
public:
    SubProcess(const std::string& id, const std::string& name);
    virtual ~SubProcess();

    virtual void start();
    virtual void cancel();
    virtual bool isComplete() const;

    /// Marks the outcome COMPLETED when the last child finishes, then bubbles up.
    virtual void onChildCompleted(WorkNode& child);

    virtual SubProcess* clone() const;

    /// Calls v.visit(*this), then accept(v) on every child.
    virtual void accept(WorkNodeVisitor& v);

    virtual WorkNodeIterator* createIterator();        ///< depth-first
    virtual WorkNodeIterator* createReadyIterator();   ///< only ready work items

    WorkflowData& getLocalData();
    const WorkflowData& getLocalData() const;
    WorkflowOutcome getOutcome() const;
    void setOutcome(WorkflowOutcome o);

protected:
    /// Deep copy: copies localData and outcome as well as the children.
    SubProcess(const SubProcess& other);

    /// Routing decisions inside a sub-process use its own local data.
    virtual const WorkflowData& routingData() const;

private:
    WorkflowData localData;
    WorkflowOutcome outcome;
};

#endif // SUBPROCESS_H