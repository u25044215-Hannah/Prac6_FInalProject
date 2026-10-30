/*Task is abstract (clone())
Work Structure's real AutomatedTask/HumanTask, will replace this once it exists.*/
#include <iostream>
#include "work_behaviour/Task.h"
#include "work_behaviour/WorkItemDecorator.h"
#include "work_behaviour/AccessControlProxy.h"

class DemoTask : public Task {
public:
    using Task::Task;
    WorkNode* clone() const override { return new DemoTask(*this); }
};

void printState(WorkItem& w) {
    std::cout << "  " << w.getName() << " -> " << w.stateName() << "\n";
}

int main() {
    Participant alice("alice");
    alice.roles.push_back(Role("Engineer"));
    Participant bob("bob"); // no roles, used to prove access control rejects

    ActionHistory history;
    WorkflowEventPublisher events;

    AccessPolicy policy;
    policy.permit("Engineer", "complete");
    policy.permit("Engineer", "assign");

    std::cout << "=== State: full lifecycle via public WorkItem interface ===\n";
    WorkItem* task = new DemoTask("t1", "Fix login bug");
    printState(*task);
    task->makeAvailable();           printState(*task); //created ->available
    task->assign(alice, alice);      printState(*task); //available -> assigned
    task->start();                   printState(*task); //assigned ->inProgress
    WorkflowData result; result.set("outcome", "fixed");
    task->complete(result, alice);   printState(*task); //inProgress-> Completed

    std::cout << "\n=== State: illegal transition rejected with a clear exception ===\n";
    try {
        task->hold(); // Completed is terminal
    } catch (const IllegalTransitionException& ex) {
        std::cout << "  Caught as expected: " << ex.what() << "\n";
    }

    std::cout << "\n=== Decorator: stacked, still a plain WorkItem to callers ===\n";
    WorkItem* raw = new DemoTask("t2", "Deploy hotfix");
    Condition always; always.field = "outcome"; always.op = "=="; always.value = "ok";
    WorkItem* decorated = new AuditDecorator(
        new PriorityDecorator(
            new NotificationDecorator(raw, always),
            "urgent", 900),
        &history);
    decorated->makeAvailable();
    decorated->assign(alice, alice);
    printState(*decorated);

    std::cout << "\n=== Proxy: allowed vs. denied actor ===\n";
    WorkItem* guarded = new AccessControlProxy(new DemoTask("t3", "Rotate secrets"), &policy, &events);
    guarded->makeAvailable();
    std::cout << "Alice (Engineer) assigns:\n";
    guarded->assign(alice, alice);
    printState(*guarded);
    guarded->start();
    WorkflowData r2; r2.set("outcome", "done");
    std::cout << "Bob (no roles) tries to complete:\n";
    guarded->complete(r2, bob);   //should be silently denied, no state change
    printState(*guarded);        // still InProgress
    std::cout << "Alice (Engineer) completes:\n";
    guarded->complete(r2, alice);
    printState(*guarded);       //now Completed

    delete task;
    delete decorated;
    delete guarded;
    return 0;
}
