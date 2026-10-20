#ifndef TIVIDY_WORKFLOW_OUTCOME_H
#define TIVIDY_WORKFLOW_OUTCOME_H

#include <string>

///overall outcome of a workflow execution
enum class WorkflowOutcome { RUNNING, COMPLETED, REJECTED, CANCELLED, FAILED };

///@return a string representation of the outcome
std::string outcomeToString(WorkflowOutcome outcome);

///@return true for every outcome except RUNNING
bool isTerminal(WorkflowOutcome outcome);

#endif // TIVIDY_WORKFLOW_OUTCOME_H