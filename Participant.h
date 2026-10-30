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

    

    void addRole(Role& role);
    void addSkill(const std::string& skill);
    void adjustWorkload(int delta);
    const std::string& getId() const {
    return ids;
}
};

#endif // PARTICIPANT_H