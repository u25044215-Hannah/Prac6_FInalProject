#ifndef DEFINITIONBUILDER_H
#define DEFINITIONBUILDER_H

#include <map>
#include <memory>
#include <stack>
#include <string>
#include "WorkflowDefinitionBuilder.h"

class WorkContainer;
class WorkflowDefinition;
class EscalationHandler;
class ApprovalHandler;

class DefinitionBuilder : public WorkflowDefinitionBuilder {
public:
    DefinitionBuilder();
    virtual ~DefinitionBuilder();

    virtual void beginDefinition(const std::string& name);
    virtual void beginStage(const std::string& name);
    virtual void beginSubProcess(const std::string& name);
    virtual void endContainer();

    virtual void addHumanTask(const std::string& id, const std::string& role);
    virtual void addAutomatedTask(const std::string& id, const std::string& connector);

    virtual void dependsOn(const std::string& id, const std::string& prereqId);
    virtual void setRouting(const std::string& key);
    virtual void setAssignment(const std::string& id, const std::string& key);
    virtual void setDeadline(const std::string& id, int seconds);
    virtual void decorateWith(const std::string& id, const std::string& key);

    virtual void addEscalationLevel(const std::string& role, int seconds);
    virtual void addApprovalLevel(const std::string& role, ApprovalLimits limits);

    /// Validates, releases the product to the caller and resets the builder.
    virtual WorkflowDefinition* build();

private:
    DefinitionBuilder(const DefinitionBuilder&) = delete;
    DefinitionBuilder& operator=(const DefinitionBuilder&) = delete;

    /// Locate a node already added to the product, throws if not found.
    WorkNode* requireNode(const std::string& id) const;
    /// Throws if beginDefinition() has not been called.
    void requireProduct() const;
    /// Discard any half-built state.
    void reset();

    std::stack<WorkContainer*> open;   // containers currently being filled
    WorkflowDefinition* product;       // definition under construction

    // Chains are built front to back and attached to the product in build().
    EscalationHandler* escalationHead;
    EscalationHandler* escalationTail;
    ApprovalHandler*   approvalHead;
    ApprovalHandler*   approvalTail;

    int nextAutoId;                    // ids for stages/sub-processes
};

#endif