#include "Participant.h"
#include "Role.h"
#include <algorithm>
#include <stdexcept>

Participant::Participant(const std::string& id) : ids(id) {}
bool Participant::hasRole(const Role& role) const {
    for (const Role* r : roles)
        if (r && r->getName() == role.getName()) return true;
    return false;
}
int Participant::getWorkload() const { return workload; }
const std::string& Participant::getId() const { return ids; }
bool Participant::hasSkill(const std::string& skill) const {
    return std::find(skills.begin(), skills.end(), skill) != skills.end();
}
void Participant::addRole(Role& role) {
    if (!hasRole(role)) roles.push_back(&role);
}
void Participant::addSkill(const std::string& skill) {
    if (!hasSkill(skill)) skills.push_back(skill);
}
void Participant::adjustWorkload(int delta) {
    if (delta < 0 && workload < -static_cast<long long>(delta))
        throw std::out_of_range("Participant workload cannot become negative");
    if (static_cast<long long>(workload) + delta > 2147483647LL)
        throw std::overflow_error("Participant workload overflow");
    workload += delta;
}
