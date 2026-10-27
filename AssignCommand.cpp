/**
 * @file AssignCommand.cpp
 * @author Janke Rall, u24571238 (Commands, History & Recovery)
 */

#include "AssignCommand.h"

#include <stdexcept>

#include "Participant.h"
#include "WorkItem.h"

AssignCommand::AssignCommand(WorkItem& item, Participant& assignee, Participant* actor)
    : WorkflowCommand(&item, actor), assignee(&assignee) {
    if (actor == nullptr) {
        throw std::invalid_argument("AssignCommand: an actor is required");
    }
}

void AssignCommand::execute() {
    receiver->assign(*assignee, *actor); // Available -> Assigned
}

void AssignCommand::undo() {
    if (isUndone()) {
        throw std::logic_error("'" + describe() + "' has already been undone");
    }
    // Withdrawing the assignment is the true inverse: Assigned -> Available. If the assignee already accepted or started, the State refuses (throws) and we do NOT mark 
    // the command as undone.
    receiver->declineAssignment(*assignee);
    markUndone();
}

bool AssignCommand::isUndoable() const {
    return true;
}

std::string AssignCommand::describe() const {
    return "Assign '" + receiver->getId() + "' to " + assignee->getId() + " (by " + actorName() + ")";
}
