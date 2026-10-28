#include "person5Forward.h"
#include "Condition.h"

class RoutingStrategy {
public:
    virtual ~RoutingStrategy() {}
    virtual std::vector<WorkNode*> nextNodes(WorkNode& done, WorkContainer& container,
                                              const WorkflowData& data) = 0;
    virtual bool isComplete(const WorkContainer& container) const = 0;
};
