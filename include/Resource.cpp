#include "Resource.h"
#include <stdexcept>

Resource::Resource(const std::string& id, const std::string& type)
    : id(id), type(type), inUse(false) {
    if (id.empty() || type.empty())
        throw std::invalid_argument("Resource id and type must not be empty");
}
const std::string& Resource::getId() const { return id; }
const std::string& Resource::getType() const { return type; }
bool Resource::isAvailable() const { return !inUse; }
void Resource::reportFailure() { inUse = true; }
void Resource::setInUse(bool value) { inUse = value; }
