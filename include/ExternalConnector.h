#ifndef EXTERNALCONNECTOR_H
#define EXTERNALCONNECTOR_H

class WorkflowData;

class ExternalConnector {
public:
    virtual ~ExternalConnector() {}

    virtual WorkflowData send(const WorkflowData& request) = 0;
    virtual bool isAvailable() const = 0;
};

#endif
