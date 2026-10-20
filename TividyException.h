#ifndef TIVIDYEXCEPTION_H
#define TIVIDYEXCEPTION_H

#include <stdexcept>
#include <string>

///base class for all exceptions thrown by Tividy
class TividyException : public std::runtime_error {
public:
    explicit TividyException(const std::string& message) : std::runtime_error(message) {}
};

///a request definition, version, instance or key does not exist
class NotFoundException : public TividyException {
public:
    explicit NotFoundException(const std::string& message) : TividyException(message) {}
};

///operation is invalid
class InvalidOperationException : public TividyException {
public:
    explicit InvalidOperationException(const std::string& message) : TividyException(message) {}
};

#endif // TIVIDYEXCEPTION_H