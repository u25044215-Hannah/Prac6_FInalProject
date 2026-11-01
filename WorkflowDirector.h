/**
 * @file WorkflowDirector.h
 * @brief Declares the project interfaces and types.
 */

#ifndef WORKFLOWDIRECTOR_H
#define WORKFLOWDIRECTOR_H

class WorkflowDefinitionBuilder;

/**
 * @brief Represents the WorkflowDirector component of the workflow engine.
 */
class WorkflowDirector {
public:
    /**
     * @brief Performs the WorkflowDirector operation.
     */
    virtual ~WorkflowDirector() {}

    /// Drive the builder. Throws std::runtime_error on malformed input.
    virtual void construct(WorkflowDefinitionBuilder& b) = 0;
};

#endif 