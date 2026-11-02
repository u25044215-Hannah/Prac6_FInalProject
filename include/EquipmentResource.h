#ifndef EQUIPMENTRESOURCE_H
#define EQUIPMENTRESOURCE_H

#include <string>
#include "Resource.h"

class EquipmentResource : public Resource {
public:
    explicit EquipmentResource(const std::string& id);
};

#endif
