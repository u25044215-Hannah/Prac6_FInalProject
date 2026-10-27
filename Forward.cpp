#include "Forward.h"

bool Condition::evaluate(const WorkflowData& data) const {
    return data.has(field) && !data.get(field).empty();
}
