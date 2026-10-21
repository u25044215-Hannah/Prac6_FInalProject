/**
 * @file CompleteCommand.h
 * @brief command that completes work item with result data
 * @author Janke Rall, u24571238 (Command, History & Recovery)
 * 
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

    CompleteCommand(WorkItem& item, const WorkflowData* result, Participant* actor);

    void execute() override;
    std::string describe() const override;
private:
    WorkflowData result;
};

#endif