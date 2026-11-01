#include "WorkflowDefinition.h"
#include "WorkflowInstance.h"
#include "WorkNode.h"
#include "Stage.h"
#include "HumanTask.h"
#include "WorkItemState.h"

// Command Pattern
#include "CompleteCommand.h"
#include "CommandProcessor.h"
#include "Participant.h"

#include <iostream>
#include <memory>
#include <exception>

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

    // =====================================================
    // CREATE WORKFLOW DEFINITION
    // =====================================================

    // Create a generic workflow definition.
    Stage* root = new Stage("root", "Workflow");

    root->add(new HumanTask("A", "First task"));

    // Task B depends on Task A.
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

    // =====================================================
    // CREATE TWO INDEPENDENT WORKFLOW INSTANCES
    // =====================================================

    WorkflowInstance instance1(
        "instance-1",
        definition,
        input
    );

    WorkflowInstance instance2(
        "instance-2",
        definition,
        input
    );

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

    // =====================================================
    // TEST 1: DEPENDENT TASK STARTS LOCKED
    // =====================================================

    check(
        "Dependent task initially locked",
        b1->stateName() == "Created"
    );

    // =====================================================
    // TEST 2: INDEPENDENT TASK STARTS IN CREATED
    // =====================================================

    check(
        "Independent task initially Created",
        c1->stateName() == "Created"
    );

    // =====================================================
    // TEST 3: AVAILABLE PREREQUISITE DOES NOT UNLOCK B
    // =====================================================

    a1->setState(new AvailableState());

    check(
        "Available prerequisite does not unlock B",
        b1->stateName() == "Created"
    );

    // =====================================================
    // TEST 4: INPROGRESS PREREQUISITE DOES NOT UNLOCK B
    // =====================================================

    a1->setState(new InProgressState());

    check(
        "InProgress prerequisite does not unlock B",
        b1->stateName() == "Created"
    );

    // =====================================================
    // TEST 5: COMPLETION AUTOMATICALLY UNLOCKS B
    // =====================================================

    // No manual publisher.publish() call is needed.
    // Task A publishes its own state-change event.
    a1->setState(new CompletedState());

    check(
        "Completed prerequisite automatically unlocks B",
        b1->stateName() == "Available"
    );

    // =====================================================
    // TEST 6: INDEPENDENT TASK REMAINS CREATED
    // =====================================================

    // DependencyResolver must not unlock tasks that
    // have no explicit prerequisites.
    check(
        "Independent task remains Created",
        c1->stateName() == "Created"
    );

    // =====================================================
    // TEST 7: INSTANCE 2 REMAINS UNAFFECTED
    // =====================================================

    check(
        "Other workflow instance remains unchanged",
        b2->stateName() == "Created"
    );

    // =====================================================
    // TEST 8: INSTANCE 2 PREREQUISITE REMAINS CREATED
    // =====================================================

    check(
        "Other instance prerequisite remains Created",
        a2->stateName() == "Created"
    );

    // =====================================================
    // TEST 9-11: COMMAND -> STATE -> OBSERVER -> DEPENDENCY
    // =====================================================

    // Use a fresh instance to prevent earlier tests
    // from interfering with command execution.
    WorkflowInstance commandInstance(
        "command-instance",
        definition,
        input
    );

    HumanTask* commandA = dynamic_cast<HumanTask*>(
        commandInstance.getRoot().findById("A")
    );

    HumanTask* commandB = dynamic_cast<HumanTask*>(
        commandInstance.getRoot().findById("B")
    );

    if (!commandA || !commandB)
    {
        std::cerr << "Command test setup failed\n";
        return 1;
    }

    // Create the participant who will complete Task A.
    Participant actor("test-participant");

    // CommandProcessor acts as the Command Pattern invoker.
    CommandProcessor processor;

    // Prepare Task A for completion.
    // This isolates the command-driven completion behaviour.
    commandA->setState(new InProgressState());

    // =====================================================
    // TEST 9: TASK B IS LOCKED BEFORE COMMAND EXECUTION
    // =====================================================

    check(
        "Command test: B initially locked",
        commandB->stateName() == "Created"
    );

    // =====================================================
    // TEST 10-11: COMPLETE TASK A THROUGH COMMAND PATTERN
    // =====================================================

    WorkflowData result;

    try
    {
        // CommandProcessor takes ownership of the command.
        // CompleteCommand calls Task::complete().
        processor.submit(
            new CompleteCommand(
                *commandA,
                result,
                &actor
            ),
            commandInstance
        );

        // TEST 10:
        // Verify that the command completed Task A.
        check(
            "CompleteCommand changes A to Completed",
            commandA->stateName() == "Completed"
        );

        // TEST 11:
        // Verify that completing A automatically
        // triggered the Observer and unlocked B.
        check(
            "CompleteCommand automatically unlocks B",
            commandB->stateName() == "Available"
        );
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "Command integration error: "
            << e.what()
            << '\n';

        // Record both tests as failed if execution throws.
        check(
            "CompleteCommand changes A to Completed",
            false
        );

        check(
            "CompleteCommand automatically unlocks B",
            false
        );
    }

    // =====================================================
    // FINAL TEST RESULTS
    // =====================================================

    std::cout
        << "\nTOTAL: "
        << passed << " passed, "
        << failed << " failed\n";

    return failed == 0 ? 0 : 1;
}