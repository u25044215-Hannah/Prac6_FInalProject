#ifndef ILLEGALTRANSITIONEXCEPTION_H
#define ILLEGALTRANSITIONEXCEPTION_H

#include <stdexcept>
#include <string>

class IllegalTransitionException : public std::logic_error {
public:
    IllegalTransitionException(const std::string& fromState, const std::string& operation)
        : std::logic_error("Illegal transition: '" + operation +
                           "' is not valid from state '" + fromState + "'") {}
};

#endif
