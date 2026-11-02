#include "Deadline.h"
#include <stdexcept>
Deadline::Deadline(const std::string& itemId, std::time_t dueAt, const std::string& kind)
    : itemId(itemId), dueAt(dueAt), kind(kind) {
    if (itemId.empty() || kind.empty())
        throw std::invalid_argument("Deadline item id and kind must not be empty");
}
const std::string& Deadline::getItemId() const { return itemId; }
std::time_t Deadline::getDueAt() const { return dueAt; }
const std::string& Deadline::getKind() const { return kind; }
