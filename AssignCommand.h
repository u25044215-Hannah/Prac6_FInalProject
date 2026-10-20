#ifndef ASSIGNCOMMAND_H
#define ASSIGNCOMMAND_H

#include "WorkflowCommand.h"

#include <iostream>
#include <string>

class AssignCommand: public WorkflowCommand {
public:
    void execute() override;
    void undo() override;
    std::string describe() const override;
};

#endif