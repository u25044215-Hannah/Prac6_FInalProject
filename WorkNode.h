#ifndef WORKNODE_H
#define WORKNODE_H

#include <string>

class WorkContainer;

class WorkNode {
public:
    virtual ~WorkNode();
    virtual void start() = 0;
    virtual void cancel() = 0;
    virtual bool isComplete() const = 0;
    virtual WorkNode* clone() const = 0;
    virtual WorkNode* findById(const std::string& nodeId);
    const std::string& getId() const;
    const std::string& getName() const;
    WorkContainer* getParent() const;
    void setParent(WorkContainer* p);

protected:
    WorkNode(const std::string& id, const std::string& name);
    WorkNode(const WorkNode& other);

    std::string id;
    std::string name;
    WorkContainer* parent;

private:
    WorkNode& operator=(const WorkNode&) = delete;
};

#endif