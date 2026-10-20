#include "doctest.h"
#include "WorkflowOutcome.h"

TEST_CASE("WorkflowOutcome names") {
    CHECK(outcomeToString(WorkflowOutcome::RUNNING) == "RUNNING");
    CHECK(outcomeToString(WorkflowOutcome::COMPLETED) == "COMPLETED");
    CHECK(outcomeToString(WorkflowOutcome::REJECTED) == "REJECTED");
    CHECK(outcomeToString(WorkflowOutcome::CANCELLED) == "CANCELLED");
    CHECK(outcomeToString(WorkflowOutcome::FAILED) == "FAILED");
}

TEST_CASE("Only RUNNING is non-terminal") {
    CHECK_FALSE(isTerminal(WorkflowOutcome::RUNNING));
    CHECK(isTerminal(WorkflowOutcome::COMPLETED));
    CHECK(isTerminal(WorkflowOutcome::REJECTED));
    CHECK(isTerminal(WorkflowOutcome::CANCELLED));
    CHECK(isTerminal(WorkflowOutcome::FAILED));
}
