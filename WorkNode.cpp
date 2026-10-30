#include "WorkNode.h"

WorkNode::WorkNode(const std::string& id, const std::string& name) 
    : id(id), name(name), parent(nullptr){}

WorkNode::WorkNode(const WorkNode& other)
    : id(other.id), name(other.name), parent(nullptr){}

WorkNode::~WorkNode(){}

WorkNode* WorkNode::findById(const std::string& nodeId){
    return (id == nodeId) ? this : nullptr;
}

const std::string& WorkNode::getId() const {return id;}
const std::string& WorkNode::getName() const {return name;}
WorkContainer* WorkNode::getParent() const {return parent;}
void WorkNode::setParent(WorkContainer* p){parent = p;}