#ifndef DEFINITIONBUILDER_H
#define DEFINITIONBUILDER_H

#include <map>
#include <memory>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>
#include "WorkflowDefinitionBuilder.h" // also provides ApprovalLimits

class Stage;
class WorkContainer;
class WorkNode;
class WorkflowDefinition;
class EscalationHandler;
class ApprovalHandler;

class DefinitionBuilder : public WorkflowDefinitionBuilder
{
public:
    DefinitionBuilder();
    virtual ~DefinitionBuilder();

    virtual void beginDefinition(const std::string &name);
    virtual void beginStage(const std::string &name);
    virtual void beginSubProcess(const std::string &name);
    virtual void endContainer();

    virtual void addHumanTask(const std::string &id, const std::string &role);
    virtual void addAutomatedTask(const std::string &id, const std::string &connector);

    virtual void dependsOn(const std::string &id, const std::string &prereqId);
    virtual void setRouting(const std::string &key);
    virtual void setAssignment(const std::string &id, const std::string &key);
    virtual void setDeadline(const std::string &id, int seconds);
    virtual void decorateWith(const std::string &id, const std::string &key);

    virtual void addEscalationLevel(const std::string &role, int seconds);
    virtual void addApprovalLevel(const std::string &role, ApprovalLimits limits);

    /// Validates, releases the product to the caller and resets the builder.
    virtual WorkflowDefinition *build();

    /// Discards a half-built definition (e.g. after build() reported errors).
    void abort();

private:
    DefinitionBuilder(const DefinitionBuilder &) = delete;
    DefinitionBuilder &operator=(const DefinitionBuilder &) = delete;

    /// Locate a node already added to the tree, throws if not found.
    WorkNode *requireNode(const std::string &id) const;
    /// Throws if beginDefinition() has not been called.
    void requireProduct() const;
    /// Throws if the id is empty or already used (does NOT reserve it).
    void checkIdFree(const std::string &id) const;
    /// Next unused auto id for stages / sub-processes (reserved '@' prefix).
    std::string newAutoId(const char *prefix);
    /// Collects every problem and throws one runtime_error. Changes nothing.
    void validate() const;
    /// Discard any half-built state.
    void reset();

    std::stack<WorkContainer *> open; // containers currently being filled
    std::string definitionName;       // name for the WorkflowDefinition
    Stage *root;                      // top of the tree (owned until build())

    // Chains: built front to back, attached to the definition in build().
    // The head owns the whole chain (handlers own `next`).
    std::shared_ptr<EscalationHandler> escalationHead;
    EscalationHandler *escalationTail;
    std::shared_ptr<ApprovalHandler> approvalHead;
    ApprovalHandler *approvalTail;

    int nextAutoId;                // counter for auto ids
    std::set<std::string> usedIds; // every id in the tree

    // Rules referring to nodes by id; checked in validate(), applied in build().
    std::vector<std::pair<std::string, std::string>> dependencies;
    std::map<std::string, std::string> assignments;
    std::map<std::string, int> deadlines;
    std::map<std::string, std::vector<std::string>> decorations;
};

#endif