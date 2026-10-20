#ifndef ESCALATECOMMAND_H
#define ESCALATECOMMAND_H

#include "WorkflowCommand.h"

#include <iostream>
#include <string>

class EscalateCommand: public WorkflowCommand {
public:
    void execute() override;
    std::string describe() const override;
};

#endif