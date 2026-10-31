#include "ParticipantDirectory.h"
#include "Participant.h"
#include "Role.h"
#include <stdexcept>

ParticipantDirectory::~ParticipantDirectory() {
    for (auto& entry : participants_) delete entry.second;
    for (auto& entry : roles_) delete entry.second;
}
void ParticipantDirectory::add(Participant* participant) {
    if (!participant) throw std::invalid_argument("Null participant");
    const std::string id = participant->getId();
    if (participants_.count(id)) throw std::invalid_argument("Duplicate participant ID");
    participants_.emplace(id, participant); // ownership transfers on success
}
void ParticipantDirectory::addRole(Role* role) {
    if (!role) throw std::invalid_argument("Null role");
    const std::string name = role->getName();
    if (roles_.count(name)) throw std::invalid_argument("Duplicate role name");
    roles_.emplace(name, role); // ownership transfers on success
}
std::vector<Participant*> ParticipantDirectory::findByRole(const Role& role) const {
    std::vector<Participant*> matches;
    for (const auto& entry : participants_)
        if (entry.second && entry.second->hasRole(role)) matches.push_back(entry.second);
    return matches;
}
Participant* ParticipantDirectory::find(const std::string& id) const {
    auto it = participants_.find(id);
    return it == participants_.end() ? nullptr : it->second;
}
