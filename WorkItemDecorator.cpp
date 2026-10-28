#include "WorkItemDecorator.h"
#include <iostream>

//notification Decorator
void NotificationDecorator::complete(WorkflowData result, Participant& actor) {
    if (notifyWhen_.evaluate(result)) {
        std::cout << "[Notification] " << getName() << " completed — condition met, notifying.\n";
    }
    inner_->complete(result, actor);
}
void NotificationDecorator::fail(const std::string& reason) {
    std::cout << "[Notification] " << getName() << " failed: " << reason << "\n";
    inner_->fail(reason);
}

//validation Decorator
void ValidationDecorator::complete(WorkflowData result, Participant& actor) {
    for (const auto& rule : rules_) {
        if (!rule.evaluate(result)) {
            /* Blocks completion rather than forwarding it
            instead, with a reason identifying which rule rejected it.*/
            inner_->fail("Validation failed: rule on field '" + rule.field + "' not satisfied");
            return;
        }
    }
    inner_->complete(result, actor);
}

//Priority Decorator
void PriorityDecorator::makeAvailable() {
    std::cout << "[Priority:" << priority_ << "] " << getName() << " now available, target completion in " << targetCompletionSeconds_ << "s.\n";
    inner_->makeAvailable();
}
void PriorityDecorator::assign(Participant& p, Participant& actor) {
    std::cout << "[Priority:" << priority_ << "] assigning " << getName() << ".\n";
    inner_->assign(p, actor);
}

//Audit Decorator
void AuditDecorator::makeAvailable() {
    if (history_) history_->appendNote(getName() + ": makeAvailable");
    inner_->makeAvailable();
}
void AuditDecorator::assign(Participant& p, Participant& actor) {
    if (history_) history_->appendNote(getName() + ": assign(" + p.id + ") by " + actor.id);
    inner_->assign(p, actor);
}
void AuditDecorator::acceptAssignment(Participant& actor) {
    if (history_) history_->appendNote(getName() + ": acceptAssignment by " + actor.id);
    inner_->acceptAssignment(actor);
}
void AuditDecorator::declineAssignment(Participant& actor) {
    if (history_) history_->appendNote(getName() + ": declineAssignment by " + actor.id);
    inner_->declineAssignment(actor);
}
void AuditDecorator::complete(WorkflowData result, Participant& actor) {
    if (history_) history_->appendNote(getName() + ": complete by " + actor.id);
    inner_->complete(result, actor);
}
void AuditDecorator::fail(const std::string& reason) {
    if (history_) history_->appendNote(getName() + ": fail - " + reason);
    inner_->fail(reason);
}
void AuditDecorator::reject(const std::string& reason, Participant& actor) {
    if (history_) history_->appendNote(getName() + ": reject by " + actor.id + " - " + reason);
    inner_->reject(reason, actor);
}
void AuditDecorator::escalate() {
    if (history_) history_->appendNote(getName() + ": escalate");
    inner_->escalate();
}
void AuditDecorator::hold() {
    if (history_) history_->appendNote(getName() + ": hold");
    inner_->hold();
}
void AuditDecorator::release() {
    if (history_) history_->appendNote(getName() + ": release");
    inner_->release();
}
