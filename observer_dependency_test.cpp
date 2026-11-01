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

    check(
        "Dependent task initially locked",
        b1->stateName() == "Created"
    );

    check(
        "Independent task initially Created",
        c1->stateName() == "Created"
    );

    a1->setState(new AvailableState());

    check(
        "Available prerequisite does not unlock B",
        b1->stateName() == "Created"
    );

    a1->setState(new InProgressState());

    check(
        "InProgress prerequisite does not unlock B",
        b1->stateName() == "Created"
    );

    // No manual publisher.publish() call is needed.
    // Task A publishes its own state-change event.
    a1->setState(new CompletedState());

    check(
        "Completed prerequisite automatically unlocks B",
        b1->stateName() == "Available"
    );

    // DependencyResolver must not unlock tasks that
    // have no explicit prerequisites.
    check(
        "Independent task remains Created",
        c1->stateName() == "Created"
    );

    check(
        "Other workflow instance remains unchanged",
        b2->stateName() == "Created"
    );

    check(
        "Other instance prerequisite remains Created",
        a2->stateName() == "Created"
    );

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

    check(
        "Command test: B initially locked",
        commandB->stateName() == "Created"
    );

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

        // Verify that the command completed Task A.
        check(
            "CompleteCommand changes A to Completed",
            commandA->stateName() == "Completed"
        );

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

    std::cout
        << "\nTOTAL: "
        << passed << " passed, "
        << failed << " failed\n";

    return failed == 0 ? 0 : 1;
}