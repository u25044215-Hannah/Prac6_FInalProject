/**
 * @file CancelRequestCommand.cpp
 * @author Janke Rall, u24571238 (Commands, History & Recovery)
 */

#include "CancelRequestCommand.h"

#include <stdexcept>

#include "include/ResourceMediator.h"
#include "WorkNode.h"
#include "WorkflowInstance.h"

CancelRequestCommand::CancelRequestCommand(WorkflowInstance& instance, const std::string& reason, Participant* actor)
    : WorkflowCommand(nullptr, actor), instance(&instance), reason(reason) {}

CancelRequestCommand::~CancelRequestCommand() {
    for (std::size_t i = 0; i < steps.size(); ++i) {
        delete steps[i];
    }
}

void CancelRequestCommand::addStep(WorkflowCommand* step) {
    if (step == nullptr) {
        throw std::invalid_argument("CancelRequestCommand::addStep: step is null");
    }
    if (step == this) {
        throw std::invalid_argument("CancelRequestCommand::addStep: cannot add itself");
    }
    steps.push_back(step);
}

void CancelRequestCommand::execute() {
    std::vector<WorkflowCommand*> done;  // steps that ran, so we can roll them back
    try {
        for (std::size_t i = 0; i < steps.size(); ++i) {
            steps[i]->execute();
            done.push_back(steps[i]);
        }
        instance->getRoot().cancel(); // Composite: cancels every active node
        //resources->release(resource); // Mediator: free what the instance held
        instance->setOutcome(WorkflowOutcome::CANCELLED);
    } catch (...) {
        // Put back what we can, newest first, so the instance is not left half-cancelled. Errors while undoing are ignored so that the ORIGINAL error is the one reported.
        for (std::vector<WorkflowCommand*>::reverse_iterator it = done.rbegin(); it != done.rend(); ++it) {
            if ((*it)->isUndoable() && !(*it)->isUndone()) {
                try {
                    (*it)->undo();
                } catch (...) {
                }
            }
        }
        throw;
    }
}

std::string CancelRequestCommand::describe() const {
    std::string text = "Cancel request " + instance->getId();
    if (!reason.empty()) {
        text += ": " + reason;
    }
    return text + " (by " + actorName() + ")";
}
