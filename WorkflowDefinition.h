/**
 * @file WorkflowDefinition.h
 * @brief The reusable, immutable-once-published template of a process.
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
 * A definition owns its template root (a WorkNode tree built by Ty's
 * builder). Instances never use this tree directly; InstanceManager
 * deep-clones it (Prototype), so running work can never change the template.
 *
 * Lifecycle: draft (mutable) -> published (immutable). A definition is
 * published by DefinitionRepository::save(), which also assigns its version.
 *
 * Ownership: the definition owns its root and shares ownership of the chain
 * heads (Hannah's CoR classes). It cannot be copied.
 */
class WorkflowDefinition {
public:
    /**
     * @param name  non-empty process name, e.g. "lab-test-request"
     * @param root  template root; must not be null
     * @throws InvalidOperationException for an empty name or null root
     */
    WorkflowDefinition(const std::string& name, std::unique_ptr<WorkNode> root);
    ~WorkflowDefinition();

    const std::string& getName() const { return name_; }
    int getVersion() const { return version_; }
    bool isPublished() const { return published_; }

    /// @return the template root (owned by this definition)
    const WorkNode& getRoot() const;

    /// @return the head of the escalation chain, or nullptr if none configured
    EscalationHandler* getEscalationChain() const { return escalation_.get(); }
    /// @return the head of the approval chain, or nullptr if none configured
    ApprovalHandler* getApprovalChain() const { return approval_.get(); }

    /// @throws InvalidOperationException if already published
    void setEscalationChain(std::shared_ptr<EscalationHandler> head);
    void setApprovalChain(std::shared_ptr<ApprovalHandler> head);

    /// Assign the version number (done by the repository). @throws InvalidOperationException if published or version < 1
    void setVersion(int version);

    /// Mark this definition as published (done by the repository). @throws InvalidOperationException if already published
    void publish();

    /// @return label name, e.g. "lab-test-request v2"
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