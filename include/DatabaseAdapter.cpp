#include "DatabaseAdapter.h"
#include "LegacyDatabaseDriver.h"
#include "WorkflowData.h"
#include <stdexcept>
DatabaseAdapter::DatabaseAdapter(LegacyDatabaseDriver* d) : driver(d) {}
bool DatabaseAdapter::isAvailable() const { return driver != nullptr; }
WorkflowData DatabaseAdapter::send(const WorkflowData& request) {
    if (!isAvailable()) throw std::runtime_error("Database driver not provided");
    const std::string result = driver->executeQuery(request.get("sql"));
    WorkflowData response;
    response.set("result", result);
    return response;
}
