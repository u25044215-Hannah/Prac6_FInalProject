
#include "vendor/doctest.h"

#include "HumanTask.h"
#include "Participant.h"
#include "Role.h"
#include "WorkflowData.h"

#include <memory>
#include <stdexcept>

TEST_CASE("State - Complete task lifecycle") {

    Role engineer("Engineer");

    Participant alice("alice");
    alice.addRole(engineer);

    std::unique_ptr<WorkItem> task(
        new HumanTask("t1", "Fix login bug", &engineer)
    );

    CHECK(task->stateName() == "Created");

    task->makeAvailable();
    CHECK(task->stateName() == "Available");

    task->assign(alice, alice);
    CHECK(task->stateName() == "Assigned");

    task->start();
    CHECK(task->stateName() == "InProgress");

    WorkflowData result;
    result.set("outcome", "fixed");

    task->complete(result, alice);
    CHECK(task->stateName() == "Completed");
}

TEST_CASE("State - Completed task rejects hold") {

    Role engineer("Engineer");

    Participant alice("alice");
    alice.addRole(engineer);

    HumanTask task("t1", "Fix login bug", &engineer);

    task.makeAvailable();
    task.assign(alice, alice);
    task.start();

    WorkflowData result;
    task.complete(result, alice);

    CHECK_THROWS(task.hold());
}

TEST_CASE("Prototype - Cloning creates independent task") {

    Role engineer("Engineer");

    HumanTask original(
        "t2", "Review deployment", &engineer
    );

    std::unique_ptr<WorkNode> copy(original.clone());

    REQUIRE(copy != nullptr);

    CHECK(copy.get() != &original);
    CHECK(copy->getName() == original.getName());
}
