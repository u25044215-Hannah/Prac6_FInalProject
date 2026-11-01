/**
 * @file EscalateCommand.cpp
 */

#include "EscalateCommand.h"

#include "WorkItem.h"

EscalateCommand::EscalateCommand(WorkItem& item, const std::string& reason, Participant* actor)
    : WorkflowCommand(&item, actor), reason(reason) {}

void EscalateCommand::execute() {
    receiver->escalate(); // Assigned/InProgress -> Escalated
}

std::string EscalateCommand::describe() const {
    std::string text = "Escalate '" + receiver->getId() + "'";
    if (!reason.empty()) {
        text += ": " + reason;
    }
    return text + " (by " + actorName() + ")";
}
