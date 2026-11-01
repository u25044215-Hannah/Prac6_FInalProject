
#include "vendor/doctest.h"

#include "DependencySet.h"
#include "Stage.h"
#include "HumanTask.h"
#include "WorkItemState.h"

// TEST 1: No dependencies
TEST_CASE("DependencySet - No prerequisites") {
    Stage root("root", "Workflow");
    DependencySet none;

    CHECK(none.isSatisfied(root));
}

// TEST 2: Incomplete prerequisite
TEST_CASE("DependencySet - Incomplete prerequisite") {
    Stage root("root", "Workflow");
    root.add(new HumanTask("A", "First task"));

    DependencySet dependencies;
    dependencies.add("A");

    CHECK_FALSE(dependencies.isSatisfied(root));
}

// TEST 3: Completed prerequisite
TEST_CASE("DependencySet - Completed prerequisite") {
    Stage root("root", "Workflow");

    auto* task = new HumanTask("A", "First task");
    root.add(task);

    DependencySet dependencies;
    dependencies.add("A");

    task->setState(new CompletedState());

    CHECK(dependencies.isSatisfied(root));
}

// TEST 4: Missing prerequisite
TEST_CASE("DependencySet - Missing prerequisite") {
    Stage root("root", "Workflow");

    DependencySet dependencies;
    dependencies.add("DOES_NOT_EXIST");

    CHECK_FALSE(dependencies.isSatisfied(root));
}

// TEST 5: Multiple prerequisites, one incomplete
TEST_CASE("DependencySet - One prerequisite incomplete") {
    Stage root("root", "Workflow");

    auto* a = new HumanTask("A", "First task");
    auto* b = new HumanTask("B", "Second task");

    root.add(a);
    root.add(b);

    a->setState(new CompletedState());

    DependencySet dependencies;
    dependencies.add("A");
    dependencies.add("B");

    CHECK_FALSE(dependencies.isSatisfied(root));
}

// TEST 6: All prerequisites completed
TEST_CASE("DependencySet - All prerequisites completed") {
    Stage root("root", "Workflow");

    auto* a = new HumanTask("A", "First task");
    auto* b = new HumanTask("B", "Second task");

    root.add(a);
    root.add(b);

    a->setState(new CompletedState());
    b->setState(new CompletedState());

    DependencySet dependencies;
    dependencies.add("A");
    dependencies.add("B");

    CHECK(dependencies.isSatisfied(root));
}
