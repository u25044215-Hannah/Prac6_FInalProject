#ifndef WORKCONTAINER_H
#define WORKCONTAINER_H

#include <string>
#include <vector>
#include "WorkNode.h"

class WorkContainer : public WorkNode
{
public:
    virtual ~WorkContainer();
    void add(WorkNode *n);
    void remove(WorkNode *n);
    const std::vector<WorkNode *> &getChildren() const;
    virtual void onChildCompleted(WorkNode &child);
    virtual bool isComplete() const;
    void setRoutingKey(const std::string &key);
    const std::string &getRoutingKey() const;
    virtual WorkNode *findById(const std::string &nodeId);

protected:
    WorkContainer(const std::string &id, const std::string &name);
    WorkContainer(const WorkContainer &other);

    std::vector<WorkNode *> children;
    std::string routingKey;
};

#endif