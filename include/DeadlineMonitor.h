#ifndef DEADLINEMONITOR_H
#define DEADLINEMONITOR_H

#include <map>
#include <string>
#include <ctime>
#include "Deadline.h"

class DeadlineMonitor {
private:
    std::multimap<std::time_t, Deadline> timers;

public:
    void startTimer(const std::string& itemId,
                    int seconds,
                    const std::string& kind);

    void cancelTimer(const std::string& itemId);

    void tick(std::time_t now);
};

#endif
