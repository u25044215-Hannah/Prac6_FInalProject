/**
 * @file AssignCommand.h
 * @brief Command that assigns a work item to a participant
 * @author Janke Rall, u24571238 (Commands, History & Recovery)
 * 
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

    void execute() override;
    void undo() override;
    bool isUndoable() const override;
    std::string describe() const override;
private:
    Participant* assignee;
};

#endif