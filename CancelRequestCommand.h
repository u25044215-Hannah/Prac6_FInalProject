#ifndef CANCELREQUESTCOMMAND_H
#define CANCELREQUESTCOMMAND_H

#include "WorkflowCommand.h"

#include <iostream>
#include <string>
#include <vector>

class CancelRequestCommand : public WorkflowCommand {
private:
    std::vector<WorkflowCommand*> steps;
public:
    void execute() override;
    void undo() override;
    std::string describe() const override;
};

#endif