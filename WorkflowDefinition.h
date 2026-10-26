/**
 * @file WorkflowDefinition.h
 * @brief The reusable, immutable-once-published template of a process
 */
#ifndef TIVIDY_WORKFLOW_DEFINITION_H
#define TIVIDY_WORKFLOW_DEFINITION_H

#include <memory>
#include <string>

class WorkNode;
class EscalationHandler;
class ApprovalHandler;

/**
 * @brief Describes how a process may behave (GoF role: Builder::Product).
 *
 * Owns the template tree, escalation and approval chains. 
 * Published definitions are immutable. 
 */
class WorkflowDefinition {
public:
    WorkflowDefinition(const std::string& name, std::unique_ptr<WorkNode> root);
    ~WorkflowDefinition();

    const std::string& getName() const { return name_; }
    int getVersion() const { return version_; }
    bool isPublished() const { return published_; }

    const WorkNode& getRoot() const;
    EscalationHandler* getEscalationChain() const { return escalation_.get(); }
    ApprovalHandler* getApprovalChain() const { return approval_.get(); }

    /// @throws InvalidOperationException if already published
    void setEscalationChain(std::shared_ptr<EscalationHandler> head);
    void setApprovalChain(std::shared_ptr<ApprovalHandler> head);

    void setVersion(int version);
    void publish();
    
    std::string label() const;

private:
    WorkflowDefinition(const WorkflowDefinition&);
    WorkflowDefinition& operator=(const WorkflowDefinition&);

    void requireDraft(const char* what) const;

    std::string name_;
    int version_;
    bool published_;
    std::unique_ptr<WorkNode> root_;
    std::shared_ptr<EscalationHandler> escalation_;
    std::shared_ptr<ApprovalHandler> approval_;
};

#endif