#ifndef RESOURCEMEDIATOR_H
#define RESOURCEMEDIATOR_H

#include <string>

class Task;
class Resource;

class ResourceMediator {
public:
    virtual ~ResourceMediator() {}

    virtual Resource* reserve(Task& requester,
                              const std::string& type,
                              int priority) = 0;

    virtual void release(Resource* r) = 0;
};

#endif
