/**
 * @file CancelRequestCommand.h
 * @brief MacroCommand that cancels a whole workflow instance.
 * @author Janke Rall, u24571238 (Commands, History & Recovery)
 * 
 * Pattern: Command -> ConcreteCommand (macro/composite command)
 */

#ifndef CANCELREQUESTCOMMAND_H
#define CANCELREQUESTCOMMAND_H

#include "WorkflowCommand.h"

#include <vector>
#include <string>
class WorkflowInstance;
class ResourceMediator;

/**
 * @brief Cancels a running workflow instance as one recorded action.
 * 
 * execute() does the following, in order:
 * 1. runs any extra steps added with addStep()
 * 2. calls cancel() on instance's root worknode. Composite forwards this to every stage, subprocess and active item
 * 3. asks the ResourceMediator to release resources help by the instance
 * 4. sets instance outcome to CANCELLED
 * 
 * work that was compoleted stays completed & in history
 * 
 * In the case of a step throwing partway, steps already executed that are undoable get undone in reverse order before exception,
 * a failed cancel thus does not leave instance half-cancelld
 * 
 * Not undoable as a whole (cancelled is final), use RecoveryManager
 */

class CancelRequestCommand : public WorkflowCommand {
public:
    /**
     * @param instance The instance to cancel (receiver)
     * @param resources mediator used to release reserved resources (receiver)
     * @param reason why rewuest was cancelld
     * @param actor who cancelled it (nullptr = external event)
     */

    CancelRequestCommand(
    WorkflowInstance& instance,
    //ResourceMediator& resources,
    const std::string& reason,
    Participant* actor
);
    /**
     * @brief deletes the owned steps
     */

    ~CancelRequestCommand() override;

     /**
      * @brief adds extra step to run before cancellation
      * @param step takes ownership, must be nullptr
      */

    void addStep(WorkflowCommand* step);

    void execute() override;
    std::string describe() const override;
private:
    WorkflowInstance* instance;
    //ResourceMediator* resources;
    std::string reason;
    std::vector<WorkflowCommand*> steps;
};

#endif