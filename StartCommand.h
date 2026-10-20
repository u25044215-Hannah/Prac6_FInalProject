#ifndef STARTCOMMAND_H
#define STARTCOMMAND_H

#include "WorkflowCommand.h"

#include <iostream>
#include <string>

class StartCommand: public WorkflowCommand {
public:
    void execute() override;
    void undo() override;
    std::string describe() const override;
};

#endif