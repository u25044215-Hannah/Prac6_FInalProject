#include "doctest.h"
#include "ApprovalHandler.h"
#include "EscalationHandler.h"
#include "FakeNodes.h"
#include "TividyException.h"

TEST_CASE("A definition needs a name and a root") {
    CHECK_THROWS_AS(WorkflowDefinition("", std::unique_ptr<WorkNode>(new FakeNode("r"))), InvalidOperationException);
    CHECK_THROWS_AS(WorkflowDefinition("x", std::unique_ptr<WorkNode>()), InvalidOperationException);
}

TEST_CASE("A draft definition is unpublished and has no version") {
    std::unique_ptr<WorkflowDefinition> def = makeDefinition("lab");
    CHECK_FALSE(def->isPublished());
    CHECK(def->getVersion() == 0);
    CHECK(def->getEscalationChain() == nullptr);
    CHECK(def->getApprovalChain() == nullptr);
}

TEST_CASE("Saving assigns increasing versions and publishes") {
    InMemoryDefinitionRepository repo;
    const WorkflowDefinition* v1 = repo.save(makeDefinition("lab"));
    const WorkflowDefinition* v2 = repo.save(makeDefinition("lab"));
    CHECK(v1->getVersion() == 1);
    CHECK(v2->getVersion() == 2);
    CHECK(v1->isPublished());
    CHECK(v2->isPublished());
    CHECK(v1 != v2);
}

TEST_CASE("Definitions are found by name and version") {
    InMemoryDefinitionRepository repo;
    const WorkflowDefinition* v1 = repo.save(makeDefinition("lab"));
    const WorkflowDefinition* v2 = repo.save(makeDefinition("lab"));
    repo.save(makeDefinition("claims"));
    CHECK(repo.find("lab", 1) == v1);
    CHECK(repo.find("lab", 2) == v2);
    CHECK(repo.latest("lab") == v2);
    CHECK(repo.latest("claims")->getVersion() == 1);
}

TEST_CASE("Unknown definitions are reported as nullptr, not a crash") {
    InMemoryDefinitionRepository repo;
    repo.save(makeDefinition("lab"));
    CHECK(repo.find("lab", 0) == nullptr);
    CHECK(repo.find("lab", 3) == nullptr);
    CHECK(repo.find("other", 1) == nullptr);
    CHECK(repo.latest("other") == nullptr);
    CHECK(repo.versionsOf("other").empty());
}

TEST_CASE("Repository lists names and versions") {
    InMemoryDefinitionRepository repo;
    repo.save(makeDefinition("lab"));
    repo.save(makeDefinition("lab"));
    repo.save(makeDefinition("claims"));
    std::vector<std::string> names = repo.names();
    REQUIRE(names.size() == 2);
    CHECK(names[0] == "claims");
    CHECK(names[1] == "lab");
    CHECK(repo.versionsOf("lab").size() == 2);
}

TEST_CASE("Repository refuses null and already-published definitions") {
    InMemoryDefinitionRepository repo;
    CHECK_THROWS_AS(repo.save(std::unique_ptr<WorkflowDefinition>()), InvalidOperationException);
    std::unique_ptr<WorkflowDefinition> def = makeDefinition("lab");
    def->setVersion(1);
    def->publish();
    CHECK_THROWS_AS(repo.save(std::move(def)), InvalidOperationException);
}

TEST_CASE("A published definition can no longer be changed") {
    InMemoryDefinitionRepository repo;
    const WorkflowDefinition* stored = repo.save(makeDefinition("lab"));
    WorkflowDefinition& def = const_cast<WorkflowDefinition&>(*stored);
    CHECK_THROWS_AS(def.setVersion(9), InvalidOperationException);
    CHECK_THROWS_AS(def.publish(), InvalidOperationException);
    CHECK_THROWS_AS(def.setApprovalChain(std::make_shared<ApprovalHandler>()), InvalidOperationException);
    CHECK_THROWS_AS(def.setEscalationChain(std::make_shared<EscalationHandler>()), InvalidOperationException);
    CHECK(stored->getVersion() == 1);
}

TEST_CASE("Chains can be attached while the definition is a draft") {
    std::unique_ptr<WorkflowDefinition> def = makeDefinition("lab");
    std::shared_ptr<ApprovalHandler> approval = std::make_shared<ApprovalHandler>();
    std::shared_ptr<EscalationHandler> escalation = std::make_shared<EscalationHandler>();
    def->setApprovalChain(approval);
    def->setEscalationChain(escalation);
    CHECK(def->getApprovalChain() == approval.get());
    CHECK(def->getEscalationChain() == escalation.get());
}

TEST_CASE("Version numbers must be at least 1") {
    std::unique_ptr<WorkflowDefinition> def = makeDefinition("lab");
    CHECK_THROWS_AS(def->setVersion(0), InvalidOperationException);
    CHECK(def->label() == "lab");
    def->setVersion(3);
    CHECK(def->label() == "lab v3");
}