#include "WorkflowDefinition.h"
#include "WorkNode.h"
#include "TividyException.h"
#include <sstream>

WorkflowDefinition::WorkflowDefinition(const std::string& name, std::unique_ptr<WorkNode> root)
    : name_(name), version_(0), published_(false), root_(std::move(root)) {
    if (name_.empty()) 
    {
        throw InvalidOperationException("A workflow definition needs a name");
    }
    if (!root_) 
    {
        throw InvalidOperationException("Workflow definition '" + name_ + "' needs a root node");
    }
}

WorkflowDefinition::~WorkflowDefinition() {}

const WorkNode& WorkflowDefinition::getRoot() const {
    return *root_;
}

void WorkflowDefinition::requireDraft(const char* what) const {
    if (published_) 
    {
        throw InvalidOperationException(std::string("Cannot ") + what + ": " + label() + " is already published");
    }
}

void WorkflowDefinition::setEscalationChain(std::shared_ptr<EscalationHandler> head) {
    requireDraft("change the escalation chain");
    escalation_ = head;
}

void WorkflowDefinition::setApprovalChain(std::shared_ptr<ApprovalHandler> head) {
    requireDraft("change the approval chain");
    approval_ = head;
}

void WorkflowDefinition::setVersion(int version) {
    requireDraft("change the version");
    if (version < 1) 
    {
        throw InvalidOperationException("Version numbers start at 1");
    }
    version_ = version;
}

void WorkflowDefinition::publish() {
    requireDraft("publish again");
    published_ = true;
}

std::string WorkflowDefinition::label() const {
    std::ostringstream out;
    out << name_;
    if (version_ > 0) out << " v" << version_;
    return out.str();
}