/**
 * @file CompleteCommand.cpp
 * @author Janke Rall, u24571238 (Commands, History & Recovery)
 */

#include "CompleteCommand.h"

#include <stdexcept>

#include "WorkItem.h"

CompleteCommand::CompleteCommand( WorkItem& item, const WorkflowData& result, Participant* actor)
    : WorkflowCommand(&item, actor), result(result)
{
}

void CompleteCommand::execute() {
    receiver->complete(result, *actor); // InProgress -> Completed (via any decorators/proxy)
}

std::string CompleteCommand::describe() const {
    return "Complete '" + receiver->getId() + "' (by " + actorName() + ")";
}
