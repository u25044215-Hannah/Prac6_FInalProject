#include "Role.h"

Role::Role(const std::string& name) : name_(name) {}
const std::string& Role::getName() const { return name_; }
