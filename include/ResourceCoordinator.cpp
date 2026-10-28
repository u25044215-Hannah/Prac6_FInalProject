#include "ResourceCoordinator.h"
#include "Resource.h"
#include <algorithm>
#include <stdexcept>

ResourceCoordinator::ResourceCoordinator() {}
ResourceCoordinator::~ResourceCoordinator() {} // Registered resources are NOT owned.

void ResourceCoordinator::addResource(Resource* r) {
    if (!r) throw std::invalid_argument("Cannot register a null resource");
    if (std::find(resources.begin(), resources.end(), r) != resources.end())
        throw std::invalid_argument("Resource is already registered");
    for (std::vector<Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
        if ((*it)->getId() == r->getId())
            throw std::invalid_argument("Resource id must be unique");
    resources.push_back(r);
}

Resource* ResourceCoordinator::reserve(Task& requester, const std::string& type, int priority) {
    if (type.empty()) throw std::invalid_argument("Resource type must not be empty");
    for (std::vector<Resource*>::iterator it = resources.begin(); it != resources.end(); ++it) {
        Resource* r = *it;
        if (r->getType() == type && r->isAvailable()) {
            r->setInUse(true);
            return r;
        }
    }
    waiting.push(ResourceRequest(&requester, type, priority));
    return nullptr;
}

void ResourceCoordinator::release(Resource* r) {
    if (!r || std::find(resources.begin(), resources.end(), r) == resources.end())
        throw std::invalid_argument("Resource is not registered");
    r->setInUse(false);
    // No Task notification/assignment API exists in these headers.
    // Pending requests cannot be fulfilled automatically by this interface.
}

void ResourceCoordinator::onResourceFailed(Resource& r) {
    if (std::find(resources.begin(), resources.end(), &r) == resources.end())
        throw std::invalid_argument("Resource is not registered");
    r.reportFailure(); // Marks unavailable; no separate failure state exists.
}
