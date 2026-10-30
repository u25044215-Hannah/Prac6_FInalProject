#include "DecoratorFactory.h"

#include <map>
#include <stdexcept>
#include <utility>

#include "WorkItem.h"

namespace
{
    typedef std::map<std::string, DecoratorFactory::Creator> Registry;

    // Function-local static avoids static-initialisation-order problems.
    Registry& registry()
    {
        static Registry r;
        return r;
    }
}

void DecoratorFactory::registerDecorator(const std::string& key, Creator creator)
{
    if (key.empty())
        throw std::invalid_argument("decorator key must not be empty");
    if (!creator)
        throw std::invalid_argument("decorator creator for '" + key + "' must not be empty");
    registry()[key] = std::move(creator);
}

bool DecoratorFactory::isKnown(const std::string& key)
{
    return registry().count(key) != 0;
}

WorkItem* DecoratorFactory::wrap(const std::string& key, WorkItem* inner)
{
    if (!inner)
        throw std::invalid_argument("cannot decorate a null work item");
    Registry::const_iterator it = registry().find(key);
    if (it == registry().end())
        throw std::invalid_argument("unknown decorator key '" + key + "'");
    return it->second(inner);
}

void DecoratorFactory::clear()
{
    registry().clear();
}