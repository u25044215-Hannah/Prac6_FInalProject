#include "LegacyDatabaseDriver.h"
#include <stdexcept>
std::string LegacyDatabaseDriver::executeQuery(const std::string&) {
    throw std::runtime_error("Database connection is not configured; integrate a database driver before use");
}
