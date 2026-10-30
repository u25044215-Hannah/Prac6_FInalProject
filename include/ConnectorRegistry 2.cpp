#include "ConnectorRegistry.h"
#include "ExternalConnector.h"
#include <stdexcept>
void ConnectorRegistry::add(const std::string& name, ExternalConnector* c) {
    if (name.empty() || !c) throw std::invalid_argument("Connector name and pointer are required");
    connectors[name] = c;
}
ExternalConnector* ConnectorRegistry::find(const std::string& name) {
    std::map<std::string, ExternalConnector*>::iterator it = connectors.find(name);
    return it == connectors.end() ? nullptr : it->second;
}
