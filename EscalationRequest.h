#ifndef ESCALATION_REQUEST_H
#define ESCALATION_REQUEST_H
#include <string>

class EscalationRequest {
private:
    std::string itemId_;
    int levelReached_ = 0;
public:
    explicit EscalationRequest(const std::string& itemId);
    const std::string& getItemId() const;
    int getLevelReached() const;
    void setLevelReached(int level);
};

#endif // ESCALATION_REQUEST_H
