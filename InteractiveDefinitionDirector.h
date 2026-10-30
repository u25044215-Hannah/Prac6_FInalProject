#ifndef INTERACTIVEDEFINITIONDIRECTOR_H
#define INTERACTIVEDEFINITIONDIRECTOR_H

#include <vector>
#include "WorkflowDirector.h"
#include "WorkflowData.h"

class InteractiveDefinitionDirector : public WorkflowDirector {
public:
    InteractiveDefinitionDirector();
    virtual ~InteractiveDefinitionDirector();

    /// Append one step. Validation happens in construct().
    void addStep(const WorkflowData& step);
    void clear();
    std::size_t stepCount() const;

    /// Throws std::runtime_error naming the offending step if one is malformed.
    virtual void construct(WorkflowDefinitionBuilder& b);

private:
    std::vector<WorkflowData> steps;
};

#endif