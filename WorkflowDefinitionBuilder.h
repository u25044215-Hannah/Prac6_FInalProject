#ifndef WORKFLOWDEFINITIONBUILDER_H
#define WORKFLOWDEFINITIONBUILDER_H

#include <string>
#include "ApprovalLimits.h" 

class WorkflowDefinition;    

class WorkflowDefinitionBuilder {
public:
    virtual ~WorkflowDefinitionBuilder() {}

    /// Start a new definition. (Added: the UML had no place to set the name.)
    virtual void beginDefinition(const std::string& name) = 0;

    //structure
    virtual void beginStage(const std::string& name) = 0;
    virtual void beginSubProcess(const std::string& name) = 0;
    virtual void endContainer() = 0;

    //leaves
    virtual void addHumanTask(const std::string& id, const std::string& role) = 0;
    virtual void addAutomatedTask(const std::string& id, const std::string& connector) = 0;

    // rules on the most recently opened container / named task
    virtual void dependsOn(const std::string& id, const std::string& prereqId) = 0;
    virtual void setRouting(const std::string& key) = 0;
    virtual void setAssignment(const std::string& id, const std::string& key) = 0;
    virtual void setDeadline(const std::string& id, int seconds) = 0;
    virtual void decorateWith(const std::string& id, const std::string& key) = 0;

    //chains
    virtual void addEscalationLevel(const std::string& role, int seconds) = 0;
    virtual void addApprovalLevel(const std::string& role, ApprovalLimits limits) = 0;

    /// Validate (DefinitionValidationVisitor) and hand over the product.
    /// Caller (TividyEngine / repository) owns the result. Throws on invalid data.
    virtual WorkflowDefinition* build() = 0;
};

#endif