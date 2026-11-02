/**
 * @file Participant.h
 * @brief Declares the project interfaces and types.
 */

#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <string>
#include <vector>

#include "Role.h"

/**
 * @brief Represents the Participant component of the workflow engine.
 */
class Participant {
private:
    std::string ids;
    int workload = 0;
    std::vector<std::string> skills;
    std::vector<Role*> roles;

public:
    /**
     * @brief Performs the getRoles operation.
     * @return The result of the operation.
     */
    const std::vector<Role*>& getRoles() const { return roles; }

    /**
     * @brief Performs the getSkills operation.
     * @return The result of the operation.
     */
    const std::vector<std::string>& getSkills() const {
    return skills;
}
    /**
     * @brief Performs the Participant operation.
     * @param id Parameter used by this operation.
     * @return The result of the operation.
     */
    explicit Participant(const std::string& id);
    /**
     * @brief Performs the hasRole operation.
     * @param role Parameter used by this operation.
     * @return The result of the operation.
     */
    bool hasRole(const Role& role) const;
    /**
     * @brief Performs the getWorkload operation.
     * @return The result of the operation.
     */
    int getWorkload() const;
    /**
     * @brief Performs the getId operation.
     * @return The result of the operation.
     */
    const std::string& getId() const;
    /**
     * @brief Performs the hasSkill operation.
     * @param skill Parameter used by this operation.
     * @return The result of the operation.
     */
    bool hasSkill(const std::string& skill) const;
    /**
     * @brief Performs the addRole operation.
     * @param role Parameter used by this operation.
     */
    void addRole(Role& role);
    /**
     * @brief Performs the addSkill operation.
     * @param skill Parameter used by this operation.
     */
    void addSkill(const std::string& skill);
    /**
     * @brief Performs the adjustWorkload operation.
     * @param delta Parameter used by this operation.
     */
    void adjustWorkload(int delta);
};

#endif // PARTICIPANT_H