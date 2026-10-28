#include "RoutingStrategy.h"
#include "person5Forward.h"
#include "Condition.h"

class ConditionalRouting : public RoutingStrategy {
private:
    std::vector<std::pair<Condition, std::string> > branches_;
public:
    std::vector<WorkNode*> nextNodes(WorkNode& done, WorkContainer& container,
                                     const WorkflowData& data) override;
    bool isComplete(const WorkContainer& container) const override;
};
