#pragma once
#include "person5Forward.h"

class MigrationStrategy {
public:
    virtual ~MigrationStrategy() {}
    virtual void migrate(WorkflowInstance& instance, const WorkflowDefinition& definition,
                         RecoveryManager& recovery) = 0;
};