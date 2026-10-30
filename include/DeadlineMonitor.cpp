#include "DeadlineMonitor.h"
#include <stdexcept>
#include <limits>

void DeadlineMonitor::startTimer(const std::string& itemId, int seconds, const std::string& kind) {
    if (seconds < 0) throw std::invalid_argument("Timer duration cannot be negative");
    const std::time_t now = std::time(nullptr);
    if (now == static_cast<std::time_t>(-1))
        throw std::runtime_error("Unable to read system time");
    // An item can have multiple timers with different kinds.
    timers.insert(std::make_pair(now + seconds, Deadline(itemId, now + seconds, kind)));
}
void DeadlineMonitor::cancelTimer(const std::string& itemId) {
    for (std::multimap<std::time_t, Deadline>::iterator it = timers.begin(); it != timers.end();) {
        if (it->second.getItemId() == itemId) it = timers.erase(it);
        else ++it;
    }
}
void DeadlineMonitor::tick(std::time_t now) {
    // Expire timers at or before 'now'. No event callback is exposed by the header.
    timers.erase(timers.begin(), timers.upper_bound(now));
}
