#ifndef RESOURCECOORDINATOR_H
#define RESOURCECOORDINATOR_H

#include <queue>
#include <string>
#include <vector>

#include "ResourceMediator.h"
#include "ResourceRequest.h"

class Resource;
class Task;

class ResourceCoordinator : public ResourceMediator {
private:
    std::priority_queue<ResourceRequest> waiting;
    std::vector<Resource*> resources;

public:
    ResourceCoordinator();
    ~ResourceCoordinator() override;

    void onResourceFailed(Resource& r);

    Resource* reserve(Task& requester,
                      const std::string& type,
                      int priority) override;

    void release(Resource* r) override;

    void addResource(Resource* r);
};

#endif
