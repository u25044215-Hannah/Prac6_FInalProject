/**
 * @file EscalateCommand.h
 * @brief command that escalates a work item
 * @author Janke Rall, u24571238 (Command, History & Recovery)
 * 
 * Pattern: Command -> ConcreteCommand
 */

#ifndef ESCALATECOMMAND_H
#define ESCALATECOMMAND_H

#include "WorkflowCommand.h"

/**
 * @brief escalates a workitem (Assigned/InProgress to Escelated) via WorkItem::escelate()
 * 
 * usually created by DeadlineMonitor when deadline expires but participant may also escalate maunally
 * who receives escalation is decided by EscalationHandler chain, not command
 * 
 * not undoable
 */

class EscalateCommand: public WorkflowCommand {
public:
    /**
     * @param item item to escalate (receiver)
     * @param reason short reason
     * @param actor participant escalating/nullptr for system
     */

    EscalateCommand(WorkItem& item, const std::string& reason, Participant* actor = nullptr);

    void execute() override;
    std::string describe() const override;
private: 
    std::string reason;
};

#endif