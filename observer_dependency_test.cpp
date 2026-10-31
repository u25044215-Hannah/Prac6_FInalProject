#include "WorkflowDefinition.h"
#include "WorkflowInstance.h"
#include "WorkNode.h"
#include "Stage.h"
#include "HumanTask.h"
#include "include/DependencyResolver.h"
#include "include/WorkflowEventPublisher.h"
#include "include/WorkflowEvent.h"

#include <iostream>
#include <memory>

int main() {
    int passed = 0;
    int failed = 0;

    auto check = [&](const char* name, bool condition) {
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

    WorkflowDefinition definition(
        "Dependency workflow",
        std::unique_ptr<WorkNode>(root)
    );

    // Two independent instances of the same workflow.
    WorkflowData input;
    WorkflowInstance instance1("instance-1", definition, input);
    WorkflowInstance instance2("instance-2", definition, input);

    DependencyResolver resolver(instance1);
    WorkflowEventPublisher publisher;

    publisher.subscribe(
        EventType::WORK_ITEM_STATE_CHANGED,
        &resolver
    );

    HumanTask* a1 = dynamic_cast<HumanTask*>(
        instance1.getRoot().findById("A")
    );

    HumanTask* b1 = dynamic_cast<HumanTask*>(
        instance1.getRoot().findById("B")
    );

    HumanTask* b2 = dynamic_cast<HumanTask*>(
        instance2.getRoot().findById("B")
    );

    if (!a1 || !b1 || !b2) {
        std::cerr << "Test setup failed\n";
        return 1;
    }

    // Test 1: B starts locked.
    check("Dependent task initially locked",
          b1->stateName() == "Created");

    // Test 2: An incomplete A must not unlock B.
    publisher.publish(WorkflowEvent(
        EventType::WORK_ITEM_STATE_CHANGED,
        "A",
        "instance-1",
        input
    ));

    check("Incomplete prerequisite does not unlock B",
          b1->stateName() == "Created");

    // Test 3: Complete A, then publish its state-change event.
    a1->setState(new CompletedState());

    publisher.publish(WorkflowEvent(
        EventType::WORK_ITEM_STATE_CHANGED,
        "A",
        "instance-1",
        input
    ));

    check("Completed prerequisite unlocks B",
          b1->stateName() == "Available");

    // Test 4: Instance 2 must remain unaffected.
    check("Other workflow instance remains unchanged",
          b2->stateName() == "Created");

    publisher.unsubscribe(
        EventType::WORK_ITEM_STATE_CHANGED,
        &resolver
    );

    std::cout << passed << " passed, "
              << failed << " failed\n";

    return failed == 0 ? 0 : 1;
}