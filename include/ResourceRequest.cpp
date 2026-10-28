#include "ResourceRequest.h"
#include <stdexcept>
ResourceRequest::ResourceRequest(Task* requester, const std::string& type, int priority)
    : requester(requester), type(type), priority(priority) {
    if (!requester || type.empty())
        throw std::invalid_argument("Resource request needs a task and a resource type");
}
Task* ResourceRequest::getRequester() const { return requester; }
const std::string& ResourceRequest::getType() const { return type; }
int ResourceRequest::getPriority() const { return priority; }
bool ResourceRequest::operator<(const ResourceRequest& other) const {
    return priority < other.priority;
}
