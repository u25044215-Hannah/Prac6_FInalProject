/**
 * @file ApproveCommand.h
 * @brief Command that records and approval decision on an approval item.
 * @author Janke Rall, u24571238 (Commands, History & Recovery) 
 * 
 * Pattern: Command -> ConcreteCommand
 */

#ifndef APPROVECOMMAND_H
#define APPROVECOMMAND_H

#include "WorkflowCommand.h"
#include "WorkflowData.h"

/**
 * @brief Approves ApprovalTask
 * 
 * Completes item with key "decision" set to "approved" in result data.
 * ApprovalTask then passes the request along its ApprovalHAndler chain 
 * (can the actor's level approve this based on the chain), notby this command.
 * 
 * Not Undoable (final): Use RecoveryManager to reverse.
 */

class ApproveCommand : public WorkflowCommand {
public:
    /**
     * @param item The approval item (receiver)
     * @param details Extra data for decision, set by execute()
     * @param actor Participant giving approval
     */

    ApproveCommand(WorkItem& item, const WorkflowData& details, Participant* actor);

    void execute() override;
    std::string describe() const override;
private: 
    WorkflowData details;
};

#endif