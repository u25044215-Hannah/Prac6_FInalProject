/**
 * @file IllegalTransitionException.h
 * @brief Declares the project interfaces and types.
 */

#ifndef ILLEGALTRANSITIONEXCEPTION_H
#define ILLEGALTRANSITIONEXCEPTION_H

#include <stdexcept>
#include <string>

/**
 * @brief Represents the IllegalTransitionException component of the workflow engine.
 */
class IllegalTransitionException : public std::logic_error {
public:
    /**
     * @brief Performs the IllegalTransitionException operation.
     * @param fromState Parameter used by this operation.
     * @param operation Parameter used by this operation.
     */
    IllegalTransitionException(const std::string& fromState, const std::string& operation)
        : std::logic_error("Illegal transition: '" + operation +
                           "' is not valid from state '" + fromState + "'") {}
};

#endif