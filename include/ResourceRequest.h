#ifndef RESOURCEREQUEST_H
#define RESOURCEREQUEST_H

#include <string>

class Task;

class ResourceRequest {
private:
    Task* requester;
    std::string type;
    int priority;

public:
    ResourceRequest(Task* requester,
                    const std::string& type,
                    int priority);

    Task* getRequester() const;
    const std::string& getType() const;
    int getPriority() const;

    bool operator<(const ResourceRequest& other) const;
};

#endif
