// COS214 - Work behaviour smoke test
// Compile as a separate test executable (this file contains main()).
#include <iostream>
#include <memory>
#include <exception>

#include "HumanTask.h"
#include "Participant.h"
#include "Role.h"
#include "WorkflowData.h"

static void printState(const WorkItem& item) {
    std::cout << "  " << item.getName() << " -> " << item.stateName() << '\n';
}

int main() {
    try {
        Role engineer("Engineer");
        Participant alice("alice");
        alice.addRole(engineer);
        Participant bob("bob");
        (void)bob; // available for later access-control tests

        std::cout << "=== State: full task lifecycle ===\n";
        std::unique_ptr<WorkItem> task(new HumanTask("t1", "Fix login bug", &engineer));
        printState(*task);
        task->makeAvailable();
        printState(*task);
        task->assign(alice, alice);
        printState(*task);
        task->start();
        printState(*task);
        WorkflowData result;
        result.set("outcome", "fixed");
        task->complete(result, alice);
        printState(*task);

        std::cout << "\n=== State: terminal-state transition ===\n";
        try {
            task->hold();
            std::cerr << "  WARNING: hold() did not throw in a terminal state\n";
        } catch (const std::exception& ex) {
            std::cout << "  Caught expected rejection: " << ex.what() << '\n';
        }

        std::cout << "\n=== Prototype: cloned task definition ===\n";
        HumanTask original("t2", "Review deployment", &engineer);
        std::unique_ptr<WorkNode> copy(original.clone());
        std::cout << "  Original: " << original.getName() << '\n';
        std::cout << "  Clone:    " << copy->getName() << '\n';
        if (copy.get() == &original) {
            std::cerr << "  ERROR: clone returned the original object\n";
            return 1;
        }
        std::cout << "  Clone is a different object\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Test failed: " << ex.what() << '\n';
        return 1;
    }
}
