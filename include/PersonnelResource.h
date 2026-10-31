#ifndef PERSONNELRESOURCE_H
#define PERSONNELRESOURCE_H

#include <string>
#include "Resource.h"

class PersonnelResource : public Resource {
public:
    explicit PersonnelResource(const std::string& id);
};

#endif
