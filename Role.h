#pragma once
#include "Person5Forward.h"

class Role {
private:
    std::string name_;
public:
    explicit Role(const std::string& name);
    const std::string& getName() const;
};
