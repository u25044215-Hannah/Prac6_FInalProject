/**
 * @file StartCommand.h
 * @brief Command that starts work on an assigned item
 * Pattern: Command -> ConcreteCommand
 */

#ifndef STARTCOMMAND_H
#define STARTCOMMAND_H

#include "WorkflowCommand.h"

/**
 * @brief Start a workitem (Assigned to InProgress) via WorkNode::start()
 * 
 * not undoable: state machine has no progress -> assigned transition. Use RecoveryManager checkpoint if a started item must be reset
 */

class StartCommand: public WorkflowCommand {
public:
    /**
     * @param item Item to start (receiver)
     * @param actor Participant starting the work
     */
    StartCommand(WorkItem& item, Participant* actor);

    /**
     * @brief Performs the execute operation.
     */
    void execute() override;
    /**
     * @brief Performs the describe operation.
     * @return The result of the operation.
     */
    std::string describe() const override;
};

#endif