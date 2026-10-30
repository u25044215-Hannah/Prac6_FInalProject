/**
 * @file WorkflowCommand.cpp
 * @brief Shared behaviour of all workflow commands.
 * @author Janke Rall, u24571238 (Commands, History & Recovery)
 */

#include "WorkflowCommand.h"

#include <stdexcept>

#include "Participant.h"

WorkflowCommand::WorkflowCommand(WorkItem* receiver, Participant* actor)
    : receiver(receiver), actor(actor), timestamp(std::time(nullptr)), undone(false) {}

WorkflowCommand::~WorkflowCommand() {}

void WorkflowCommand::undo() {
    // Default for every command that does not override undo().
    throw std::logic_error("'" + describe() + "' cannot be undone");
}

bool WorkflowCommand::isUndoable() const {
    return false;
}

bool WorkflowCommand::isUndone() const {
    return undone;
}

Participant* WorkflowCommand::getActor() const {
    return actor;
}

std::time_t WorkflowCommand::getTimestamp() const {
    return timestamp;
}

void WorkflowCommand::markUndone() {
    undone = true;
}

std::string WorkflowCommand::actorName() const {
    return actor != nullptr ? actor->getId() : "system";
}
