/**
 * @file ParticipantDirectory.h
 * @brief Declares the project interfaces and types.
 */

#pragma once
//#include "person5Forward.h"
#include <map>
#include <string>
#include "Participant.h"
#include "Role.h"

/**
 * @brief Represents the ParticipantDirectory component of the workflow engine.
 */
class ParticipantDirectory {
private:
    std::map<std::string, Participant*> participants_;
    std::map<std::string, Role*> roles_;
public:
    /**
     * @brief Performs the ParticipantDirectory operation.
     */
    ParticipantDirectory() = default;
    /**
     * @brief Performs the ParticipantDirectory operation.
     */
    ~ParticipantDirectory(); // owns participants and roles, consistent with UML composition
    /**
     * @brief Performs the ParticipantDirectory operation.
     */
    ParticipantDirectory(const ParticipantDirectory&) = delete;
    ParticipantDirectory& operator=(const ParticipantDirectory&) = delete;
    /**
     * @brief Performs the add operation.
     * @param participant Parameter used by this operation.
     */
    void add(Participant* participant);
    /**
     * @brief Performs the addRole operation.
     * @param role Parameter used by this operation.
     */
    void addRole(Role* role);
    /**
     * @brief Performs the findByRole operation.
     * @param role Parameter used by this operation.
     * @return The result of the operation.
     */
    std::vector<Participant*> findByRole(const Role& role) const;
    /**
     * @brief Performs the find operation.
     * @param id Parameter used by this operation.
     * @return The result of the operation.
     */
    Participant* find(const std::string& id) const;
};
