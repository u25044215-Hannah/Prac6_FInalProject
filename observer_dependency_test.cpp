#include "WorkflowDefinition.h"
#include "WorkflowInstance.h"
#include "WorkNode.h"
#include "Stage.h"
#include "HumanTask.h"
#include "WorkItemState.h"

#include <iostream>
#include <memory>

int main()
{
    int passed = 0;
    int failed = 0;

    auto check = [&](const char* name, bool condition)
    {
        std::cout << (condition ? "[PASS] " : "[FAIL] ")
                  << name << '\n';

        condition ? ++passed : ++failed;
    };

    // Create a generic workflow definition.
    Stage* root = new Stage("root", "Workflow");

    root->add(new HumanTask("A", "First task"));

    HumanTask* b = new HumanTask("B", "Second task");
    b->addDependency("A");
    root->add(b);

    // Task C has no explicit dependencies.
    HumanTask* c = new HumanTask("C", "Independent task");
    root->add(c);

    WorkflowDefinition definition(
        "Dependency workflow",
        std::unique_ptr<WorkNode>(root)
    );

    WorkflowData input;

    // Create two independent instances of the same definition.
    WorkflowInstance instance1("instance-1", definition, input);
    WorkflowInstance instance2("instance-2", definition, input);

    HumanTask* a1 = dynamic_cast<HumanTask*>(
        instance1.getRoot().findById("A")
    );

    HumanTask* b1 = dynamic_cast<HumanTask*>(
        instance1.getRoot().findById("B")
    );

    HumanTask* c1 = dynamic_cast<HumanTask*>(
        instance1.getRoot().findById("C")
    );

    HumanTask* a2 = dynamic_cast<HumanTask*>(
        instance2.getRoot().findById("A")
    );

    HumanTask* b2 = dynamic_cast<HumanTask*>(
        instance2.getRoot().findById("B")
    );

    if (!a1 || !b1 || !c1 || !a2 || !b2)
    {
        std::cerr << "Test setup failed\n";
        return 1;
    }

    // TEST 1: Dependent task starts locked.
    check(
        "Dependent task initially locked",
        b1->stateName() == "Created"
    );

    // TEST 2: Independent task starts in Created.
    check(
        "Independent task initially Created",
        c1->stateName() == "Created"
    );

    // TEST 3: Changing A to Available must not unlock B.
    a1->setState(new AvailableState());

    check(
        "Available prerequisite does not unlock B",
        b1->stateName() == "Created"
    );

    // TEST 4: Changing A to InProgress must not unlock B.
    a1->setState(new InProgressState());

    check(
        "InProgress prerequisite does not unlock B",
        b1->stateName() == "Created"
    );

    // TEST 5:
    // Completing A must AUTOMATICALLY publish an event.
    // We do NOT manually call publisher.publish().
    a1->setState(new CompletedState());

    check(
        "Completed prerequisite automatically unlocks B",
        b1->stateName() == "Available"
    );

    // TEST 6:
    // Tasks without explicit dependencies should not
    // be unlocked by DependencyResolver.
    check(
        "Independent task remains Created",
        c1->stateName() == "Created"
    );

    // TEST 7:
    // Instance 2 must remain unaffected by instance 1.
    check(
        "Other workflow instance remains unchanged",
        b2->stateName() == "Created"
    );

    // TEST 8:
    // Task A in instance 2 should also remain unchanged.
    check(
        "Other instance prerequisite remains Created",
        a2->stateName() == "Created"
    );

    std::cout << "\nTOTAL: "
              << passed << " passed, "
              << failed << " failed\n";

    return failed == 0 ? 0 : 1;
}