#pragma once
#include "person5Forward.h"

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
