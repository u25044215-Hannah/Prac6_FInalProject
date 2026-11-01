/**
 * @file TividyException.h
 * @brief Declares the project interfaces and types.
 */

#ifndef TIVIDYEXCEPTION_H
#define TIVIDYEXCEPTION_H

#include <stdexcept>
#include <string>

///base class for all exceptions thrown by Tividy
class TividyException : public std::runtime_error {
public:
    /**
     * @brief Performs the TividyException operation.
     * @param message Parameter used by this operation.
     * @return The result of the operation.
     */
    explicit TividyException(const std::string& message) : std::runtime_error(message) {}
};

///a request definition, version, instance or key does not exist
class NotFoundException : public TividyException {
public:
    /**
     * @brief Performs the NotFoundException operation.
     * @param message Parameter used by this operation.
     * @return The result of the operation.
     */
    explicit NotFoundException(const std::string& message) : TividyException(message) {}
};

///operation is invalid
class InvalidOperationException : public TividyException {
public:
    /**
     * @brief Performs the InvalidOperationException operation.
     * @param message Parameter used by this operation.
     * @return The result of the operation.
     */
    explicit InvalidOperationException(const std::string& message) : TividyException(message) {}
};

#endif // TIVIDYEXCEPTION_H