/**
 * @file InteractiveDefinitionDirector.h
 * @brief Declares the project interfaces and types.
 */

#ifndef INTERACTIVEDEFINITIONDIRECTOR_H
#define INTERACTIVEDEFINITIONDIRECTOR_H

#include <vector>
#include "WorkflowDirector.h"
#include "WorkflowData.h"
#include "DefinitionCommands.h"

/**
 * @brief Represents the InteractiveDefinitionDirector component of the workflow engine.
 */
class InteractiveDefinitionDirector : public WorkflowDirector {
public:
    /**
     * @brief Performs the InteractiveDefinitionDirector operation.
     */
    InteractiveDefinitionDirector();
    /**
     * @brief Performs the InteractiveDefinitionDirector operation.
     */
    virtual ~InteractiveDefinitionDirector();

    /// Append one step. Validation happens in construct().
    void addStep(const WorkflowData& step);
    /**
     * @brief Performs the clear operation.
     */
    void clear();
    /**
     * @brief Performs the stepCount operation.
     * @return The result of the operation.
     */
    std::size_t stepCount() const;

    /// Throws std::runtime_error naming the offending step if one is malformed.
    virtual void construct(WorkflowDefinitionBuilder& b);

private:
    std::vector<WorkflowData> steps;
};

#endif