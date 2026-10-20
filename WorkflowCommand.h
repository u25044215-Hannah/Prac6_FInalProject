#ifndef WORKFLOWCOMMAND_H
#define WORKFLOWCOMMAND_H

#include <string>
#include <iostream>

class WorkflowCommand {
protected:
    Participant* actor;
    time_t timestamp;
public:
    virtual void execute() = 0;
    virtual std::string describe() const = 0;
    virtual void undo();
    virtual bool isUndoable() const;
};

#endif