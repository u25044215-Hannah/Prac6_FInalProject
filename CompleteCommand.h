/**
 * @file CompleteCommand.h
 * @brief command that completes work item with result data 
 * Pattern: Command -> ConcreteCommand
 */

#ifndef COMPLETECOMMAND_H
#define COMPLETECOMMAND_H

#include "WorkflowCommand.h"
#include "WorkflowData.h"

/**
 * @brief completes a workitem (InProgress to Completed) and supplies its result
 * 
 * call passes through any decorators (validation, notification, audit) and access-control proxy befor it reaches the task
 * 
 * not undoable: completed is a final state
 * recovery after wrong completion uses RecoveryManager::rollback()
 */

class CompleteCommand: public WorkflowCommand {
public:
    /**
     * @param item item to complete (receiver)
     * @param result output data produced by work, merged into instance's workflowdata by task
     * @param actor participant completing work
     */

    CompleteCommand(WorkItem& item, const WorkflowData& result, Participant* actor);

    /**
     * @brief Performs the execute operation.
     */
    void execute() override;
    /**
     * @brief Performs the describe operation.
     * @return The result of the operation.
     */
    std::string describe() const override;
private:
    WorkflowData result;
};

#endif