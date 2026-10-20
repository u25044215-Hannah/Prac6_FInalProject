/**
 * Test doubles used by the core-engine tests. They stand in for Person 2's
 * real Composite classes so Person 1's tests do not depend on their code.
 */
#ifndef TIVIDY_TEST_FAKE_NODES_H
#define TIVIDY_TEST_FAKE_NODES_H

#include "InMemoryDefinitionRepository.h"
#include "MigrationStrategy.h"
#include "WorkNode.h"
#include "WorkflowCommand.h"
#include "WorkflowDefinition.h"
#include "WorkflowInstance.h"
#include <memory>
#include <string>
#include <vector>

///tiny work tree node with observable, per-node mutable state
class FakeNode : public WorkNode {
public:
    explicit FakeNode(const std::string& id) : id_(id), started_(false), done_(false), cancelled_(false) {}

    std::string getId() const { return id_; }
    void start() {
        started_ = true;
        for (std::size_t i = 0; i < children_.size(); ++i) children_[i]->start();
    }
    void cancel() { cancelled_ = true; }
    bool isComplete() const { return done_; }

    WorkNode* clone() const {
        FakeNode* copy = new FakeNode(id_);
        copy->started_ = started_;
        copy->done_ = done_;
        copy->cancelled_ = cancelled_;
        for (std::size_t i = 0; i < children_.size(); ++i) {
            copy->children_.push_back(std::unique_ptr<FakeNode>(static_cast<FakeNode*>(children_[i]->clone())));
        }
        return copy;
    }

    void captureState(std::map<std::string, std::string>& out) const {
        out[id_] = done_ ? "COMPLETED" : (started_ ? "STARTED" : "CREATED");
        for (std::size_t i = 0; i < children_.size(); ++i) children_[i]->captureState(out);
    }
    void restoreState(const std::map<std::string, std::string>& in) {
        std::map<std::string, std::string>::const_iterator it = in.find(id_);
        if (it != in.end()) {
            done_ = (it->second == "COMPLETED");
            started_ = (it->second != "CREATED");
        }
        for (std::size_t i = 0; i < children_.size(); ++i) children_[i]->restoreState(in);
    }

    FakeNode* addChild(const std::string& id) {
        children_.push_back(std::unique_ptr<FakeNode>(new FakeNode(id)));
        return children_.back().get();
    }
    FakeNode* child(std::size_t i) { return children_[i].get(); }
    void complete() { done_ = true; }
    bool started() const { return started_; }
    bool cancelled() const { return cancelled_; }

private:
    std::string id_;
    bool started_;
    bool done_;
    bool cancelled_;
    std::vector<std::unique_ptr<FakeNode> > children_;
};

///build a definition whose template is root "root" with two children "a" and "b".
inline std::unique_ptr<WorkflowDefinition> makeDefinition(const std::string& name = "demo") {
    std::unique_ptr<FakeNode> root(new FakeNode("root"));
    root->addChild("a");
    root->addChild("b");
    return std::unique_ptr<WorkflowDefinition>(new WorkflowDefinition(name, std::move(root)));
}

///command that counts how often it ran
class CountingCommand : public WorkflowCommand {
public:
    explicit CountingCommand(int* counter) : counter_(counter) {}
    void execute() { ++*counter_; }
    std::string describe() const { return "counting command"; }
private:
    int* counter_;
};

///migration policy that records which instances it was asked to migrate
class RecordingMigration : public MigrationStrategy {
public:
    std::vector<std::string> migratedIds;
    void migrate(WorkflowInstance& inst, const WorkflowDefinition&, RecoveryManager&) {
        migratedIds.push_back(inst.getId());
    }
    std::string name() const { return "recording"; }
};

#endif