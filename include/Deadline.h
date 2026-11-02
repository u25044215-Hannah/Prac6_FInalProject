#ifndef DEADLINE_H
#define DEADLINE_H

#include <string>
#include <ctime>

class Deadline {
private:
    std::string itemId;
    std::time_t dueAt;
    std::string kind;

public:
    Deadline(const std::string& itemId,
             std::time_t dueAt,
             const std::string& kind);

    const std::string& getItemId() const;
    std::time_t getDueAt() const;
    const std::string& getKind() const;
};

#endif
