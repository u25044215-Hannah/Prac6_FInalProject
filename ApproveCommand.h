#ifndef APPROVECOMMAND_H
#define APPROVECOMMAND_H

#include "WorkflowCommand.h"

#include <iostream>
#include <string>

class ApproveCommand: public WorkflowCommand {
public:
    void execute() override;
    std::string describe() const override;
};

#endif