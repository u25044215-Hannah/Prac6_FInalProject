#ifndef COMPLETECOMMAND_H
#define COMPLETECOMMAND_H

#include "WorkflowCommand.h"

#include <iostream>
#include <string>

class CompleteCommand: public WorkflowCommand {
public:
    void execute() override;
    void undo() override;
    std::string describe() const override;
};

#endif