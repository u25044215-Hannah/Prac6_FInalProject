#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <iostream>

class CommandProcessor {
public:
    void submit(WorkflowCommand* cmd, WorkflowInstance& inst);
    void undoLast(Workflowinstance& inst);
};

#endif