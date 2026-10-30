#include "EscalationRequest.h"
#include <stdexcept>

EscalationRequest::EscalationRequest(const std::string& itemId) : itemId_(itemId) {}
const std::string& EscalationRequest::getItemId() const { return itemId_; }
int EscalationRequest::getLevelReached() const { return levelReached_; }
void EscalationRequest::setLevelReached(int level) {
    if (level < 0) throw std::invalid_argument("Escalation level must be non-negative");
    levelReached_ = level;
}
