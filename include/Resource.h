#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

class Resource {
private:
    std::string id;
    std::string type;
    bool inUse;

public:
    Resource(const std::string& id,
             const std::string& type);

    virtual ~Resource() {}

    const std::string& getId() const;
    const std::string& getType() const;

    bool isAvailable() const;
    void reportFailure();

    void setInUse(bool value);
};

#endif
