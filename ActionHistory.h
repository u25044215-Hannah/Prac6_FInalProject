/**
 * @file ActionHistory.h
 * @brief Append-only audit trail of one workflow instance.
 * @author Janke Rall, u24571238 (Commands, History & Recovery)
 */

#ifndef TIVIDY_ACTIONHISTORY_H
#define TIVIDY_ACTIONHISTORY_H

#include <cstddef>
#include <string>
#include <vector>

class WorkflowCommand;

/**
 * @brief Ordered, append-only record of every action on a WorkflowInstance.
 *
 * Holds two things:
 *  - the executed WorkflowCommand objects (owned - deleted in the destructor), so they can be inspected and undone later;
 *  - a readable log in chronological order, containing a line for each command plus free-text notes (failed attempts, undo, rollbacks).
 *
 * Nothing is ever removed: an undo or a rollback is recorded as a new note, so the audit trail is complete. A WorkflowInstance owns exactly one ActionHistory 
 * (composition). It is deliberately NOT part of an InstanceSnapshot, so rolling back never erases history.
 */

class ActionHistory {
public:
    ActionHistory();

    /** @brief Deletes every command this history owns. */
    ~ActionHistory();

    /**
     * @brief Records an executed command and takes ownership of it.
     * @param cmd A command that has already executed successfully. Must not be nullptr; throws std::invalid_argument if it is.
     */
    void append(WorkflowCommand* cmd);

    /** @brief Records a free-text event, e.g. "Rolled back to 'before-run'". */
    void appendNote(const std::string& text);

    /** @brief Executed commands, oldest first (used by undo and reports). */
    const std::vector<WorkflowCommand*>& entries() const;

    /** @brief Readable timeline of commands and notes, oldest first (used by the TUI). */
    const std::vector<std::string>& log() const;

    /** @brief Number of executed commands recorded. */
    std::size_t size() const;

    /** @brief True if no command has been recorded yet. */
    bool empty() const;

private:
    std::vector<WorkflowCommand*> commands;  //Owned.
    std::vector<std::string> lines;          // Chronological log.

    // Owns raw pointers, so copying would cause a double delete.
    ActionHistory(const ActionHistory&) = delete;
    ActionHistory& operator=(const ActionHistory&) = delete;
};

#endif
