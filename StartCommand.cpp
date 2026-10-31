/**
 * @file StartCommand.cpp
 * @author Janke Rall, u24571238 (Commands, History & Recovery)
 */

#include "StartCommand.h"

#include "WorkItem.h"

StartCommand::StartCommand(WorkItem& item, Participant* actor)
    : WorkflowCommand(&item, actor) {}

void StartCommand::execute() {
    receiver->start(); // Assigned -> InProgress
}

std::string StartCommand::describe() const {
    return "Start '" + receiver->getId() + "' (by " + actorName() + ")";
}
