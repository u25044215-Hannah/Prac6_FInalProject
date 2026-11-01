/**
 * @file AssignCommand.h
 * @brief Command that assigns a work item to a participant
 * Pattern: Command -> ConcreteCommand
 */

#ifndef ASSIGNCOMMAND_H
#define ASSIGNCOMMAND_H

#include "WorkflowCommand.h"

/**
 * @brief Assigns WorkItem to participant (Available to Assigned)
 * 
 * Undoable: undo() withdraws assignment by calling
 * WorkItem: declineAssignment, which returns the item to Available
 * The State pattern guards this. If assignee has accepted or started the item
 * state refuses and undo() throws.
 */

class AssignCommand : public WorkflowCommand {
public:
    /**
     * @param item Item to assign (receiver)
     * @param assignee Participant who will do the work
     * @param actor Who made assignment (nullptr = AssignmentService)
     */
    
    AssignCommand(WorkItem& item, Participant& assignee, Participant* actor);

    /**
     * @brief Performs the execute operation.
     */
    void execute() override;
    /**
     * @brief Performs the undo operation.
     */
    void undo() override;
    /**
     * @brief Performs the isUndoable operation.
     * @return The result of the operation.
     */
    bool isUndoable() const override;
    /**
     * @brief Performs the describe operation.
     * @return The result of the operation.
     */
    std::string describe() const override;
private:
    Participant* assignee;
};

#endif