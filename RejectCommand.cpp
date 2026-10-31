/**
 * @file RejectCommand.cpp
 * @author Janke Rall, u24571238 (Commands, History & Recovery)
 */

#include "RejectCommand.h"

#include <stdexcept>

#include "WorkItem.h"

RejectCommand::RejectCommand(WorkItem& item, const std::string& reason, Participant* actor)
    : WorkflowCommand(&item, actor), reason(reason) {
    if (actor == nullptr) {
        throw std::invalid_argument("RejectCommand: an actor is required");
    }
    if (reason.empty()) {
        throw std::invalid_argument("RejectCommand: a reason is required");
    }
}

void RejectCommand::execute() {
    receiver->reject(reason, *actor); // InProgress -> Rejected
}

std::string RejectCommand::describe() const {
    return "Reject '" + receiver->getId() + "': " + reason + " (by " + actorName() + ")";
}
