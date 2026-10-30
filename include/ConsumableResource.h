#ifndef CONSUMABLERESOURCE_H
#define CONSUMABLERESOURCE_H

#include <string>
#include "Resource.h"

class ConsumableResource : public Resource {
public:
    explicit ConsumableResource(const std::string& id);
};

#endif
