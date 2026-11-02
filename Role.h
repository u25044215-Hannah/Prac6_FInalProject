/**
 * @file Role.h
 * @brief Declares the project interfaces and types.
 */

#ifndef ROLE_H
#define ROLE_H

#include <string>

/**
 * @brief Represents the Role component of the workflow engine.
 */
class Role {
private:
    std::string name_;
public:
    /**
     * @brief Performs the Role operation.
     * @param name Parameter used by this operation.
     * @return The result of the operation.
     */
    explicit Role(const std::string& name);
    /**
     * @brief Performs the getName operation.
     * @return The result of the operation.
     */
    const std::string& getName() const;
};

#endif // ROLE_H
