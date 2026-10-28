/**
 * @file RejectCommand.h
 * @brief Command that rejects a work itemm with a reason
 * @author Janke Rall, u24571238 (Command, History & Command)
 * 
 * Pattern: Command -> ConcreteCommand 
 */

#ifndef REJECTCOMMAND_H
#define REJECTCOMMAND_H

#include "WorkflowCommand.h"

/**
 * @brief Rejects a WorkItem (InProgress to Rejected) via WorkItem::reject()
 * 
 * Readson is stored in history so rejection can be audited and shown to whoever must rework the item
 * 
 * not undoable (rejected is final); use RecoveryManager to reverse
 */

class RejectCommand: public WorkflowCommand {
public:
    /**
     * @param item Item to reject (receiver)
     * @param reason Why it was rejected. Must no be empty; the constructor throws std::invalid_argument if it is
     * @param actor Participant rejecting the item
     */

    RejectCommand(WorkItem& item, const std::string& reason, Participant* actor);

    void execute() override;
    std::string describe() const override;
private:
    std::string reason;
};

#endif