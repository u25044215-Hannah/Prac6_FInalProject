#include "doctest.h"
#include "FakeNodes.h"
#include "InstanceManager.h"
#include "TividyException.h"

namespace {
struct Fixture {
    InMemoryDefinitionRepository repo;
    InstanceManager manager;
    const WorkflowDefinition* def;
    Fixture() : def(repo.save(makeDefinition("lab"))) {}
    static FakeNode& root(WorkflowInstance* i) { return static_cast<FakeNode&>(i->getRoot()); }
};
}

TEST_CASE("Instances get deterministic, unique ids") {
    Fixture f;
    WorkflowInstance* a = f.manager.createInstance(*f.def, WorkflowData());
    WorkflowInstance* b = f.manager.createInstance(*f.def, WorkflowData());
    CHECK(a->getId() == "WF-0001");
    CHECK(b->getId() == "WF-0002");
    CHECK(f.manager.count() == 2);
}

TEST_CASE("A new instance is running and not yet started") {
    Fixture f;
    WorkflowInstance* a = f.manager.createInstance(*f.def, WorkflowData());
    CHECK(a->getOutcome() == WorkflowOutcome::RUNNING);
    CHECK_FALSE(a->isStarted());
    CHECK(a->getDefinitionName() == "lab");
    CHECK(a->getDefinitionVersion() == 1);
    CHECK(a->getHistory().size() == 1);   // "created from ..."
}

TEST_CASE("Instance work trees are independent of each other") {
    Fixture f;
    WorkflowInstance* a = f.manager.createInstance(*f.def, WorkflowData());
    WorkflowInstance* b = f.manager.createInstance(*f.def, WorkflowData());
    Fixture::root(a).child(0)->complete();
    CHECK(Fixture::root(a).child(0)->isComplete());
    CHECK_FALSE(Fixture::root(b).child(0)->isComplete());
}

TEST_CASE("Running an instance never changes the definition's template") {
    Fixture f;
    WorkflowInstance* a = f.manager.createInstance(*f.def, WorkflowData());
    a->start();
    Fixture::root(a).child(1)->complete();
    const FakeNode& templateRoot = static_cast<const FakeNode&>(f.def->getRoot());
    CHECK_FALSE(templateRoot.started());
    // a new instance created afterwards starts from the pristine template
    WorkflowInstance* c = f.manager.createInstance(*f.def, WorkflowData());
    CHECK_FALSE(Fixture::root(c).started());
    CHECK_FALSE(Fixture::root(c).child(1)->isComplete());
    CHECK(&a->getRoot() != &f.def->getRoot());
}

TEST_CASE("Instance data is copied from the input and isolated") {
    Fixture f;
    WorkflowData input;
    input.set("patient", "P-1");
    WorkflowInstance* a = f.manager.createInstance(*f.def, input);
    WorkflowInstance* b = f.manager.createInstance(*f.def, input);
    a->getData().set("patient", "CHANGED");
    CHECK(b->getData().get("patient") == "P-1");
    CHECK(input.get("patient") == "P-1");
    CHECK(a->getData().get("patient") == "CHANGED");
}

TEST_CASE("Starting an instance starts its own tree only") {
    Fixture f;
    WorkflowInstance* a = f.manager.createInstance(*f.def, WorkflowData());
    WorkflowInstance* b = f.manager.createInstance(*f.def, WorkflowData());
    a->start();
    CHECK(a->isStarted());
    CHECK(Fixture::root(a).started());
    CHECK_FALSE(Fixture::root(b).started());
}

TEST_CASE("An instance cannot be started twice or after it finished") {
    Fixture f;
    WorkflowInstance* a = f.manager.createInstance(*f.def, WorkflowData());
    a->start();
    CHECK_THROWS_AS(a->start(), InvalidOperationException);
    WorkflowInstance* b = f.manager.createInstance(*f.def, WorkflowData());
    b->setOutcome(WorkflowOutcome::CANCELLED);
    CHECK_THROWS_AS(b->start(), InvalidOperationException);
}

TEST_CASE("Unpublished definitions cannot be instantiated") {
    InstanceManager manager;
    std::unique_ptr<WorkflowDefinition> draft = makeDefinition("draft");
    CHECK_THROWS_AS(manager.createInstance(*draft, WorkflowData()), InvalidOperationException);
    CHECK(manager.count() == 0);
}

TEST_CASE("Unknown instance ids are handled") {
    Fixture f;
    CHECK(f.manager.find("WF-9999") == nullptr);
    CHECK_THROWS_AS(f.manager.get("WF-9999"), NotFoundException);
    WorkflowInstance* a = f.manager.createInstance(*f.def, WorkflowData());
    CHECK(f.manager.find(a->getId()) == a);
    CHECK(&f.manager.get(a->getId()) == a);
}

TEST_CASE("Outcome starts RUNNING and a final outcome cannot be overwritten") {
    Fixture f;
    WorkflowInstance* a = f.manager.createInstance(*f.def, WorkflowData());
    a->setOutcome(WorkflowOutcome::COMPLETED);
    CHECK(a->getOutcome() == WorkflowOutcome::COMPLETED);
    CHECK(a->isFinished());
    a->setOutcome(WorkflowOutcome::COMPLETED);   // same value: ignored
    CHECK_THROWS_AS(a->setOutcome(WorkflowOutcome::FAILED), InvalidOperationException);
    CHECK(a->getOutcome() == WorkflowOutcome::COMPLETED);
}

TEST_CASE("Outcome changes are written to the instance history") {
    Fixture f;
    WorkflowInstance* a = f.manager.createInstance(*f.def, WorkflowData());
    std::size_t before = a->getHistory().size();
    a->setOutcome(WorkflowOutcome::REJECTED);
    CHECK(a->getHistory().size() == before + 1);
}

TEST_CASE("runningInstancesOf lists only running instances of that version") {
    InMemoryDefinitionRepository repo;
    InstanceManager manager;
    const WorkflowDefinition* v1 = repo.save(makeDefinition("lab"));
    const WorkflowDefinition* v2 = repo.save(makeDefinition("lab"));
    WorkflowInstance* a = manager.createInstance(*v1, WorkflowData());
    WorkflowInstance* b = manager.createInstance(*v1, WorkflowData());
    WorkflowInstance* c = manager.createInstance(*v2, WorkflowData());
    b->setOutcome(WorkflowOutcome::COMPLETED);
    std::vector<WorkflowInstance*> running1 = manager.runningInstancesOf(*v1);
    REQUIRE(running1.size() == 1);
    CHECK(running1[0] == a);
    CHECK(manager.runningInstancesOf(*v2).size() == 1);
    CHECK(manager.runningInstancesOf("lab").size() == 2);
    CHECK(manager.runningInstancesOf("other").empty());
    (void)c;
}

TEST_CASE("all() returns instances in creation order") {
    Fixture f;
    WorkflowInstance* a = f.manager.createInstance(*f.def, WorkflowData());
    WorkflowInstance* b = f.manager.createInstance(*f.def, WorkflowData());
    std::vector<WorkflowInstance*> all = f.manager.all();
    REQUIRE(all.size() == 2);
    CHECK(all[0] == a);
    CHECK(all[1] == b);
}