#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <string>
#include <vector>

#include "Role.h"

class Participant {
private:
    std::string ids;
    int workload = 0;
    std::vector<std::string> skills;
    std::vector<Role*> roles;

public:
    const std::vector<std::string>& getSkills() const {
    return skills;
}
    explicit Participant(const std::string& id);
    bool hasRole(const Role& role) const;
    int getWorkload() const;
    const std::string& getId() const;
    bool hasSkill(const std::string& skill) const;
    void addRole(Role& role);
    void addSkill(const std::string& skill);
    void adjustWorkload(int delta);
};

#endif // PARTICIPANT_H