/**
 * @file CommandProcessor.cpp
 * @brief Invoker that executes, records and undoes workflow commands.
 * @author Janke Rall, u24571238 (Commands, History & Recovery)
 */

#include "CommandProcessor.h"

#include <memory>
#include <stdexcept>

#include "ActionHistory.h"
#include "WorkflowCommand.h"
#include "WorkflowInstance.h"

CommandProcessor::CommandProcessor() {}

CommandProcessor::~CommandProcessor() {}

void CommandProcessor::submit(WorkflowCommand* cmd, WorkflowInstance& inst) {
    if (cmd == nullptr) {
        throw std::invalid_argument("CommandProcessor::submit: command is null");
    }

    // We own cmd from here. The guard deletes it automatically on every path where it does not end up in the history.
    std::unique_ptr<WorkflowCommand> guard(cmd);
    ActionHistory& history = inst.getHistory();

    try {
        cmd->execute();
    } catch (const std::exception& e) {
        history.appendNote("FAILED: " + cmd->describe() + " - " + e.what());
        throw; // guard deletes cmd; the caller (e.g. the TUI) reports the error
    } catch (...) {
        history.appendNote("FAILED: " + cmd->describe() + " - unknown error");
        throw;
    }

    history.append(cmd); // the history now owns cmd...
    guard.release(); // ...so the guard must not delete it
}

void CommandProcessor::undoLast(WorkflowInstance& inst) {
    WorkflowCommand* cmd = findLastUndoable(inst);
    if (cmd == nullptr) {
        throw std::logic_error("Nothing to undo for instance " + inst.getId());
    }

    ActionHistory& history = inst.getHistory();
    try {
        cmd->undo(); // the item's State may still refuse, e.g. if work has started
    } catch (const std::exception& e) {
        history.appendNote("UNDO FAILED: " + cmd->describe() + " - " + e.what());
        throw;
    }
    // The original entry stays: the history is append-only.
    history.appendNote("UNDONE: " + cmd->describe());
}

bool CommandProcessor::canUndo(WorkflowInstance& inst) const {
    return findLastUndoable(inst) != nullptr;
}

WorkflowCommand* CommandProcessor::findLastUndoable(WorkflowInstance& inst) const {
    const std::vector<WorkflowCommand*>& done = inst.getHistory().entries();
    // Walk backwards from the newest command.
    for (std::vector<WorkflowCommand*>::const_reverse_iterator it = done.rbegin();
         it != done.rend(); ++it) {
        if ((*it)->isUndoable() && !(*it)->isUndone()) {
            return *it;
        }
    }
    return nullptr;
}
