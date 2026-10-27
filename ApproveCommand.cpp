/**
 * @file ApproveCommand.cpp
 * @author Janke Rall, u24571238 (Commands, History & Recovery)
 */

#include "ApproveCommand.h"

#include <stdexcept>

#include "Participant.h"
#include "WorkItem.h"

ApproveCommand::ApproveCommand(WorkItem& item, const WorkflowData& details, Participant* actor)
    : WorkflowCommand(&item, actor), details(details) {
    if (actor == nullptr) {
        throw std::invalid_argument("ApproveCommand: an actor is required");
    }
}

void ApproveCommand::execute() {
    // Copy, so executing twice (e.g. in a test) never changes the stored details.
    WorkflowData decision = details;
    decision.set("decision", "approved");
    decision.set("decidedBy", actor->getId());
    // The ApprovalTask forwards this through its ApprovalHandler chain.
    receiver->complete(decision, *actor);
}

std::string ApproveCommand::describe() const {
    return "Approve '" + receiver->getId() + "' (by " + actorName() + ")";
}
