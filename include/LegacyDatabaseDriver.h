#ifndef LEGACYDATABASEDRIVER_H
#define LEGACYDATABASEDRIVER_H

#include <string>

class LegacyDatabaseDriver {
public:
    std::string executeQuery(const std::string& sql);
};

#endif
