#ifndef SUBPROCESS_H
#define SUBPROCESS_H

#include "WorkContainer.h"
#include "WorkflowData.h"    
#include "WorkflowOutcome.h"

class SubProcess : public WorkContainer {
public:
    SubProcess(const std::string& id, const std::string& name);
    virtual ~SubProcess();

    virtual void start();
    virtual void cancel();
    virtual bool isComplete() const;

    virtual SubProcess* clone() const;

    WorkflowData& getLocalData();
    const WorkflowData& getLocalData() const;
    WorkflowOutcome getOutcome() const;
    void setOutcome(WorkflowOutcome o);

protected:
    SubProcess(const SubProcess& other);

private:
    WorkflowData localData;
    WorkflowOutcome outcome;
};

#endif // SUBPROCESS_H