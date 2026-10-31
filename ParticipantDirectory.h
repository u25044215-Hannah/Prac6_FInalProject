#pragma once
//#include "person5Forward.h"
#include <map>
#include <string>
#include "Participant.h"
#include "Role.h"

class ParticipantDirectory {
private:
    std::map<std::string, Participant*> participants_;
    std::map<std::string, Role*> roles_;
public:
    ParticipantDirectory() = default;
    ~ParticipantDirectory(); // owns participants and roles, consistent with UML composition
    ParticipantDirectory(const ParticipantDirectory&) = delete;
    ParticipantDirectory& operator=(const ParticipantDirectory&) = delete;
    void add(Participant* participant);
    void addRole(Role* role);
    std::vector<Participant*> findByRole(const Role& role) const;
    Participant* find(const std::string& id) const;
};
