#include "WorkflowOutcome.h"

std::string outcomeToString(WorkflowOutcome outcome) {
    switch (outcome) {
        case WorkflowOutcome::RUNNING:   return "RUNNING";
        case WorkflowOutcome::COMPLETED: return "COMPLETED";
        case WorkflowOutcome::REJECTED:  return "REJECTED";
        case WorkflowOutcome::CANCELLED: return "CANCELLED";
        case WorkflowOutcome::FAILED:    return "FAILED";
    }
    return "UNKNOWN";
}

bool isTerminal(WorkflowOutcome outcome) {
    return outcome != WorkflowOutcome::RUNNING;
}