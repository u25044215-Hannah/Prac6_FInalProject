/**
 * @file CommandProcessor.h
 * @brief invoker that executes, records and undoes workflow commands
 * @author Janke Rall, u24571238 (Commands, History & Recovery)
 * 
 * Pattern: Command -> Invoker
 */

#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

class WorkflowCommand;
class WorkflowInstance;

/** 
 * @brief single entry point through which every action on an instance runs
 * 
 * since all actions go through submit(), every action is recorded in instance's ActionHistory in the same way, regardless of where it came from
 * TividyEngine owns one CommandProcessor and passes it to whoever needs it
 * 
 * ownership rule: the caller creates the command with @c new and gives up ownership at submit()
 * the processor either hands it to instance's ActionHistory (succes) or deletes it (failure)
 * caller must never use/delete the pointer after
 */

class CommandProcessor {
public:
    CommandProcessor();
    ~CommandProcessor();

    /**
     * @brief executes a command against an instance and records it
     * 
     * on success command is appended to inst.getHistory()
     * on failure (execute() throws) a "FAILED: ..." note is appended, command is deleted, exception is rethrown so caller can report it
     * 
     * @param cmd command to run,. ownership is transferred, throws std::invalid_argument if nullptr
     * @param inst instance command belongs to 
     */

    void submit(WorkflowCommand* cmd, WorkflowInstance& inst);

    /**
     * @brief undoes most recent undoable command on this instance that has not been undone yet
     * 
     * appends an "UNDONE: ..." note
     * original command stays in history (append-only)
     * throws std::logic_error if there's nothing to undo
     */

    void undoLast(WorkflowInstance& inst);

    /**
     * @brief most recent undoable, not yet undone command, or nullptr
     */

    bool canUndo(WorkflowInstance* inst) const;
private:
    /**
     * @brief true if undoLast() would find a command to undo / nullptr
     */
    WorkflowCommand* findLastUndoable(WorkflowInstance& isnt) const;
    CommandProcessor(const CommandProcessor&) = delete;
    CommandProcessor& operator=(const CommandProcessor&) = delete;
};

#endif