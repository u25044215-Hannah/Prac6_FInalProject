#ifndef WORKFLOWDIRECTOR_H
#define WORKFLOWDIRECTOR_H

class WorkflowDefinitionBuilder;

class WorkflowDirector {
public:
    virtual ~WorkflowDirector() {}

    /// Drive the builder. Throws std::runtime_error on malformed input.
    virtual void construct(WorkflowDefinitionBuilder& b) = 0;
};

#endif 