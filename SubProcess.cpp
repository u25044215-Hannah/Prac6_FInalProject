#include "SubProcess.h"

#include "WorkNodeVisitor.h"     // Person 7
#include "DepthFirstIterator.h"  // Person 7
#include "ReadyItemIterator.h"   // Person 7

SubProcess::SubProcess(const std::string& id, const std::string& name)
    : WorkContainer(id, name), localData(), outcome(WorkflowOutcome::RUNNING) {}

SubProcess::SubProcess(const SubProcess& other)
    : WorkContainer(other), localData(other.localData), outcome(other.outcome) {}

SubProcess::~SubProcess() {}

void SubProcess::start() {
    outcome = WorkflowOutcome::RUNNING;
    startChildren();
}

void SubProcess::cancel() {
    outcome = WorkflowOutcome::CANCELLED;
    for (std::size_t i = 0; i < children.size(); ++i)
        if (!children[i]->isComplete()) children[i]->cancel();
}

bool SubProcess::isComplete() const {
    return outcome != WorkflowOutcome::RUNNING || WorkContainer::isComplete();
}

void SubProcess::onChildCompleted(WorkNode& child) {
    // Record our own result first so the base class bubbles up correctly.
    if (outcome == WorkflowOutcome::RUNNING && WorkContainer::isComplete())
        outcome = WorkflowOutcome::COMPLETED;
    WorkContainer::onChildCompleted(child);
}

SubProcess* SubProcess::clone() const { return new SubProcess(*this); }

void SubProcess::accept(WorkNodeVisitor& v) {
    v.visit(*this);
    for (std::size_t i = 0; i < children.size(); ++i) children[i]->accept(v);
}

WorkNodeIterator* SubProcess::createIterator() {
    return new DepthFirstIterator(this);       // INTEGRATION: Person 7 ctor(WorkNode*)
}

WorkNodeIterator* SubProcess::createReadyIterator() {
    return new ReadyItemIterator(this);        // INTEGRATION: Person 7 ctor(WorkNode*)
}

const WorkflowData& SubProcess::routingData() const { return localData; }

WorkflowData& SubProcess::getLocalData() { return localData; }
const WorkflowData& SubProcess::getLocalData() const { return localData; }
WorkflowOutcome SubProcess::getOutcome() const { return outcome; }
void SubProcess::setOutcome(WorkflowOutcome o) { outcome = o; }