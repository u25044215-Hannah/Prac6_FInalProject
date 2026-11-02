#ifndef CONNECTORREGISTRY_H
#define CONNECTORREGISTRY_H

#include <map>
#include <string>

class ExternalConnector;

class ConnectorRegistry {
private:
    std::map<std::string, ExternalConnector*> connectors;

public:
    void add(const std::string& name, ExternalConnector* c);
    ExternalConnector* find(const std::string& name);
};

#endif
