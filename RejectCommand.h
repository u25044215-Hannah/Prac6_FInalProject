#ifndef REJECTCOMMAND_H
#define REJECTCOMMAND_H

#include "WorkflowCommand.h"

#include <iostream>
#include <string>

class RejectCommand: public WorkflowCommand {
public:
    void execute() override;
    std::string describe() const override;
};

#endif