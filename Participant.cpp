#include "Participant.h"
#include "Role.h"
#include <algorithm>
#include <stdexcept>

Participant::Participant(const std::string& id) : id_(id) {}
bool Participant::hasRole(const Role& role) const {
    for (const Role* r : roles_)
        if (r && r->getName() == role.getName()) return true;
    return false;
}
int Participant::getWorkload() const { return workload_; }
const std::string& Participant::getId() const { return id_; }
bool Participant::hasSkill(const std::string& skill) const {
    return std::find(skills_.begin(), skills_.end(), skill) != skills_.end();
}
void Participant::addRole(Role& role) {
    if (!hasRole(role)) roles_.push_back(&role);
}
void Participant::addSkill(const std::string& skill) {
    if (!hasSkill(skill)) skills_.push_back(skill);
}
void Participant::adjustWorkload(int delta) {
    if (delta < 0 && workload_ < -static_cast<long long>(delta))
        throw std::out_of_range("Participant workload cannot become negative");
    if (static_cast<long long>(workload_) + delta > 2147483647LL)
        throw std::overflow_error("Participant workload overflow");
    workload_ += delta;
}
