/**
 * @file WorkflowDefinitionBuilder.h
 * @brief Declares the project interfaces and types.
 */

#ifndef WORKFLOW_DEFINITION_BUILDER_H
#define WORKFLOW_DEFINITION_BUILDER_H

#include <string>
#include "ApprovalLimits.h" 

class WorkflowDefinition;    

/**
 * @brief Represents the WorkflowDefinitionBuilder component of the workflow engine.
 */
class WorkflowDefinitionBuilder {
public:
    /**
     * @brief Performs the WorkflowDefinitionBuilder operation.
     */
    virtual ~WorkflowDefinitionBuilder() {}

    /// Start a new definition. (Added: the UML had no place to set the name.)
    virtual void beginDefinition(const std::string& name) = 0;

    //structure
    /**
     * @brief Performs the beginStage operation.
     * @param name Parameter used by this operation.
     */
    virtual void beginStage(const std::string& name) = 0;
    /**
     * @brief Performs the beginSubProcess operation.
     * @param name Parameter used by this operation.
     */
    virtual void beginSubProcess(const std::string& name) = 0;
    /**
     * @brief Performs the endContainer operation.
     */
    virtual void endContainer() = 0;

    //leaves
    /**
     * @brief Performs the addHumanTask operation.
     * @param id Parameter used by this operation.
     * @param role Parameter used by this operation.
     */
    virtual void addHumanTask(const std::string& id, const std::string& role) = 0;
    /**
     * @brief Performs the addAutomatedTask operation.
     * @param id Parameter used by this operation.
     * @param connector Parameter used by this operation.
     */
    virtual void addAutomatedTask(const std::string& id, const std::string& connector) = 0;

    // rules on the most recently opened container / named task
    /**
     * @brief Performs the dependsOn operation.
     * @param id Parameter used by this operation.
     * @param prereqId Parameter used by this operation.
     */
    virtual void dependsOn(const std::string& id, const std::string& prereqId) = 0;
    /**
     * @brief Performs the setRouting operation.
     * @param key Parameter used by this operation.
     */
    virtual void setRouting(const std::string& key) = 0;
    /**
     * @brief Performs the setAssignment operation.
     * @param id Parameter used by this operation.
     * @param key Parameter used by this operation.
     */
    virtual void setAssignment(const std::string& id, const std::string& key) = 0;
    /**
     * @brief Performs the setDeadline operation.
     * @param id Parameter used by this operation.
     * @param seconds Parameter used by this operation.
     */
    virtual void setDeadline(const std::string& id, int seconds) = 0;
    /**
     * @brief Performs the decorateWith operation.
     * @param id Parameter used by this operation.
     * @param key Parameter used by this operation.
     */
    virtual void decorateWith(const std::string& id, const std::string& key) = 0;

    //chains
    /**
     * @brief Performs the addEscalationLevel operation.
     * @param role Parameter used by this operation.
     * @param seconds Parameter used by this operation.
     */
    virtual void addEscalationLevel(const std::string& role, int seconds) = 0;
    /**
     * @brief Performs the addApprovalLevel operation.
     * @param role Parameter used by this operation.
     * @param limits Parameter used by this operation.
     */
    virtual void addApprovalLevel(const std::string& role, ApprovalLimits limits) = 0;

    /// Validate (DefinitionValidationVisitor) and hand over the product.
    /// Caller (TividyEngine / repository) owns the result. Throws on invalid data.
    virtual WorkflowDefinition* build() = 0;
};

#endif