#ifndef DATABASEADAPTER_H
#define DATABASEADAPTER_H

#include "ExternalConnector.h"

class LegacyDatabaseDriver;

class DatabaseAdapter : public ExternalConnector {
private:
    LegacyDatabaseDriver* driver;

public:
    explicit DatabaseAdapter(LegacyDatabaseDriver* driver);

    WorkflowData send(const WorkflowData& request) override;
    bool isAvailable() const override;
};

#endif
