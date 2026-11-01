#include "DependencySet.h"
#include "Stage.h"
#include "HumanTask.h"
#include <iostream>

int main() {
    int passed = 0;
    int failed = 0;

    auto check = [&](const char* name, bool ok) {
        std::cout << (ok ? "[PASS] " : "[FAIL] ")
                  << name << '\n';

        if (ok) ++passed;
        else ++failed;
    };

    Stage root("root", "Workflow");

    HumanTask* a = new HumanTask("A", "First task");
    HumanTask* b = new HumanTask("B", "Second task");

    root.add(a);
    root.add(b);

    // TEST 1: No prerequisites
    DependencySet none;
    check("No dependencies", none.isSatisfied(root));

    // TEST 2: Task A is incomplete
    DependencySet dependsOnA;
    dependsOnA.add("A");

    check("Incomplete prerequisite",
          !dependsOnA.isSatisfied(root));

    // TEST 3: Task A is completed
    a->setState(new CompletedState());

    check("Completed prerequisite",
          dependsOnA.isSatisfied(root));

    // TEST 4: Prerequisite does not exist
    DependencySet missing;
    missing.add("DOES_NOT_EXIST");

    check("Missing prerequisite",
          !missing.isSatisfied(root));

    // TEST 5: Two prerequisites, one incomplete
    DependencySet multiple;
    multiple.add("A");
    multiple.add("B");

    check("Multiple prerequisites, one incomplete",
          !multiple.isSatisfied(root));

    // TEST 6: Both prerequisites completed
    b->setState(new CompletedState());

    check("Multiple prerequisites, all completed",
          multiple.isSatisfied(root));

    std::cout << passed << " passed, "
              << failed << " failed\n";

    return failed == 0 ? 0 : 1;
}