
#include "vendor/doctest.h"

#include "WorkflowDefinition.h"
#include "WorkflowInstance.h"
#include "WorkNode.h"
#include "Stage.h"
#include "HumanTask.h"
#include "WorkItemState.h"

#include "CompleteCommand.h"
#include "CommandProcessor.h"
#include "Participant.h"

#include <memory>

// ======================================================
// OBSERVER + DEPENDENCY TESTS
// ======================================================

TEST_CASE("Observer - Automatically unlock dependent tasks") {

    Stage* root = new Stage("root", "Workflow");

    root->add(new HumanTask("A", "First task"));

    HumanTask* b = new HumanTask("B", "Second task");
    b->addDependency("A");
    root->add(b);

    root->add(new HumanTask("C", "Independent task"));

    WorkflowDefinition definition(
        "Dependency workflow",
        std::unique_ptr<WorkNode>(root)
    );

    WorkflowData input;

    WorkflowInstance instance1(
        "instance-1", definition, input
    );

    WorkflowInstance instance2(
        "instance-2", definition, input
    );

    auto* a1 = dynamic_cast<HumanTask*>(
        instance1.getRoot().findById("A")
    );

    auto* b1 = dynamic_cast<HumanTask*>(
        instance1.getRoot().findById("B")
    );

    auto* c1 = dynamic_cast<HumanTask*>(
        instance1.getRoot().findById("C")
    );

    auto* a2 = dynamic_cast<HumanTask*>(
        instance2.getRoot().findById("A")
    );

    auto* b2 = dynamic_cast<HumanTask*>(
        instance2.getRoot().findById("B")
    );

    REQUIRE(a1 != nullptr);
    REQUIRE(b1 != nullptr);
    REQUIRE(c1 != nullptr);
    REQUIRE(a2 != nullptr);
    REQUIRE(b2 != nullptr);

    CHECK(b1->stateName() == "Created");
    CHECK(c1->stateName() == "Created");

    a1->setState(new AvailableState());

    CHECK(b1->stateName() == "Created");

    a1->setState(new InProgressState());

    CHECK(b1->stateName() == "Created");

    a1->setState(new CompletedState());

    CHECK(b1->stateName() == "Available");
    CHECK(c1->stateName() == "Created");

    CHECK(b2->stateName() == "Created");
    CHECK(a2->stateName() == "Created");
}

// ======================================================
// COMMAND -> STATE -> OBSERVER -> DEPENDENCY
// ======================================================

TEST_CASE("Observer - CompleteCommand unlocks dependent task") {

    Stage* root = new Stage("root", "Workflow");

    root->add(new HumanTask("A", "First task"));

    HumanTask* b = new HumanTask("B", "Second task");
    b->addDependency("A");
    root->add(b);

    WorkflowDefinition definition(
        "Command dependency workflow",
        std::unique_ptr<WorkNode>(root)
    );

    WorkflowData input;

    WorkflowInstance instance(
        "command-instance", definition, input
    );

    auto* taskA = dynamic_cast<HumanTask*>(
        instance.getRoot().findById("A")
    );

    auto* taskB = dynamic_cast<HumanTask*>(
        instance.getRoot().findById("B")
    );

    REQUIRE(taskA != nullptr);
    REQUIRE(taskB != nullptr);

    CHECK(taskB->stateName() == "Created");

    Participant actor("test-participant");
    CommandProcessor processor;

    taskA->setState(new InProgressState());

    WorkflowData result;

    REQUIRE_NOTHROW(
        processor.submit(
            new CompleteCommand(*taskA, result, &actor),
            instance
        )
    );

    CHECK(taskA->stateName() == "Completed");
    CHECK(taskB->stateName() == "Available");
}
