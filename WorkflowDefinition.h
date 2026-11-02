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
    /**
     * @brief Performs the WorkflowDefinition operation.
     * @param name Parameter used by this operation.
     * @param root Parameter used by this operation.
     */
    WorkflowDefinition(const std::string& name, std::unique_ptr<WorkNode> root);
    /**
     * @brief Performs the WorkflowDefinition operation.
     */
    ~WorkflowDefinition();

    /**
     * @brief Performs the getName operation.
     * @return The result of the operation.
     */
    const std::string& getName() const { return name_; }
    /**
     * @brief Performs the getVersion operation.
     * @return The result of the operation.
     */
    int getVersion() const { return version_; }
    /**
     * @brief Performs the isPublished operation.
     * @return The result of the operation.
     */
    bool isPublished() const { return published_; }

    /**
     * @brief Performs the getRoot operation.
     * @return The result of the operation.
     */
    const WorkNode& getRoot() const;
    /**
     * @brief Performs the getEscalationChain operation.
     * @return The result of the operation.
     */
    EscalationHandler* getEscalationChain() const { return escalation_.get(); }
    /**
     * @brief Performs the getApprovalChain operation.
     * @return The result of the operation.
     */
    ApprovalHandler* getApprovalChain() const { return approval_.get(); }

    /// @throws InvalidOperationException if already published
    void setEscalationChain(std::shared_ptr<EscalationHandler> head);
    /**
     * @brief Performs the setApprovalChain operation.
     * @param head Parameter used by this operation.
     */
    void setApprovalChain(std::shared_ptr<ApprovalHandler> head);

    /**
     * @brief Performs the setVersion operation.
     * @param version Parameter used by this operation.
     */
    void setVersion(int version);
    /**
     * @brief Performs the publish operation.
     */
    void publish();
    
    /**
     * @brief Performs the label operation.
     * @return The result of the operation.
     */
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