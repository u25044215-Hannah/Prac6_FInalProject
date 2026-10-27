#pragma once
#include "Person5Forward.h"

class Participant {
private:
    std::string id_;
    int workload_ = 0;
    std::vector<std::string> skills_;
    std::vector<Role*> roles_; // non-owning; directory manages roles
public:
    explicit Participant(const std::string& id);
    bool hasRole(const Role& role) const;
    int getWorkload() const;
    const std::string& getId() const;
    bool hasSkill(const std::string& skill) const;
    void addRole(Role& role);
    void addSkill(const std::string& skill);
    void adjustWorkload(int delta);
};
