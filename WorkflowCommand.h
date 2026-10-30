/**
 * @file WorkflowCommand.h
 * @brief  Abstract Command for every recorded action in a workflow instance
 * @author Janke Rall, u24571238 (Command, History & Recovery)
 * 
 * Pattern: Command -> Command
 */

#ifndef WORKFLOWCOMMAND_H
#define WORKFLOWCOMMAND_H

#include <string>
#include <ctime>

class Participant;
class WorkItem;

/**
 * @brief An action on workflow work, encapsulated as an object.
 *
 * Every action that changes a workflow (assign, start, complete, approve, reject, escalate, cancel) is a WorkflowCommand. This lets the engine execute actions from any 
 * source (a participant in the TUI, a deadline timer, an event listener) in one way, record them in the instance's ActionHistory for audit, and undo the ones that can be 
 * reversed.
 *
 * Lifetime: a command is created with @c new by the client, handed to CommandProcessor::submit(), and from then on owned by the ActionHistory of the instance it ran on. 
 * Clients must never delete a command after submitting it.
 *
 * Undo: only commands whose isUndoable() returns true may be undone. Actions that move a WorkItem into a final state (Completed, Rejected, Cancelled, Failed) cannot be 
 * reversed through the State pattern; they are recovered with RecoveryManager::rollback() instead.
 */

class WorkflowCommand {
public:
    /**
     * @param receiver The work item this command acts on, or nullptr for commands that act on a whole instance (CancelRequestCommand)
     * @param actor Participant performing the action, or nullptr when the systemn triggers it (e.g. deadline expirt)
     */

    WorkflowCommand(WorkItem* receiver, Participant* actor);

    /** @brief Vritual so subclasses are destroyed correctly through a abase pointer */
    virtual ~WorkflowCommand();

    /** @brief Preforms the action on receiver. May throw if receiver's current state does not allow it (IllegalTransitionException) */
    virtual void execute() = 0;

    /** @brief One-line description for history and TUI */
    virtual std::string describe() const = 0;

    /** @brief Reverses a previously executed action. Default implementation throws std::logic_error, since most commands are not undoable. Undoable subclasses override
     * this and must call markUndone() when they succeed.
     */
    virtual void undo();

    /** @brief True id undo() is supported. Default -> False */
    virtual bool isUndoable() const;

    /** @brief True once this command has been undone, so it is never undone twice. */
    bool isUndone() const;

    /** @brief The participant who issued the command (nullptr = system). */
    Participant* getActor() const;

    /** @brief When the command was created (i.e. when the action was requested). */
    std::time_t getTimestamp() const;
protected:
    /** @brief Called by subclasses at the end of a successful undo(). */
    void markUndone();

    /** @brief Readable name of the actor for describe(), "system" if none. */
    std::string actorName() const;

    WorkItem* receiver;      // Not owned. nullptr for instance-level commands.
    Participant* actor;      // Not owned. nullptr when the system acts.
    std::time_t timestamp;   // Set in the constructor.
private:
    bool undone;

    // Commands are recorded objects with an identity; copying one would
    // duplicate an audit entry.
    WorkflowCommand(const WorkflowCommand&) = delete;
    WorkflowCommand& operator=(const WorkflowCommand&) = delete;
};

#endif