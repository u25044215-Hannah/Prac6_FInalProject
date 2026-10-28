#include "WorkContainer.h"

#include <algorithm>
#include <stdexcept>

#include "RoutingStrategy.h"    // Person 5
#include "WorkflowData.h"       // Person 1
#include "LevelOrderIterator.h" // Person 7

WorkContainer::WorkContainer(const std::string &id, const std::string &name)
    : WorkNode(id, name), children(), routingKey("sequential"),
      routing(), nextToStart(0) {}

// Deep copy (Prototype): clone each child and attach the copy to this container.
WorkContainer::WorkContainer(const WorkContainer &other)
    : WorkNode(other), children(), routingKey(other.routingKey),
      routing(other.routing), nextToStart(0)
{
    try
    {
        for (std::size_t i = 0; i < other.children.size(); ++i)
        {
            WorkNode *copy = other.children[i]->clone();
            children.push_back(copy);
            copy->setParent(this);
        }
    }
    catch (...)
    {
        // The destructor will not run for a half-built object: clean up here.
        for (std::size_t i = 0; i < children.size(); ++i)
            delete children[i];
        children.clear();
        throw;
    }
}

WorkContainer::~WorkContainer()
{
    for (std::size_t i = 0; i < children.size(); ++i)
        delete children[i];
}

void WorkContainer::add(WorkNode *n)
{
    if (!n)
        throw std::invalid_argument("WorkContainer::add: null node");
    if (n == this)
        throw std::logic_error("WorkContainer::add: a container cannot contain itself");
    if (n->getParent())
        throw std::logic_error("WorkContainer::add: node '" + n->getId() + "' already has a parent");
    children.push_back(n);
    n->setParent(this);
}

void WorkContainer::remove(WorkNode *n)
{
    std::vector<WorkNode *>::iterator it = std::find(children.begin(), children.end(), n);
    if (it == children.end())
        return;
    std::size_t index = static_cast<std::size_t>(it - children.begin());
    children.erase(it);
    if (index < nextToStart)
        --nextToStart;
    delete n;
}

void WorkContainer::replaceChild(WorkNode *oldNode, WorkNode *replacement)
{
    if (!replacement)
        throw std::invalid_argument("WorkContainer::replaceChild: null replacement");
    std::vector<WorkNode *>::iterator it = std::find(children.begin(), children.end(), oldNode);
    if (it == children.end())
        throw std::logic_error("WorkContainer::replaceChild: node is not a child of this container");
    *it = replacement;
    replacement->setParent(this);
    // oldNode is intentionally NOT deleted or re-parented: the replacement
    // (a decorator) now owns it, and it still reports completion to this container.
}

const std::vector<WorkNode *> &WorkContainer::getChildren() const { return children; }

void WorkContainer::startChildren()
{
    if (children.empty())
        return;
    if (routingKey == "parallel")
    {
        nextToStart = children.size();
        for (std::size_t i = 0; i < children.size(); ++i)
            children[i]->start();
    }
    else
    {
        nextToStart = 1; // set BEFORE starting: start() may complete synchronously
        children[0]->start();
    }
}

void WorkContainer::onChildCompleted(WorkNode &child)
{
    if (routing)
    {
        // Strategy decides what runs next.
        std::vector<WorkNode *> next = routing->nextNodes(child, *this, routingData());
        for (std::size_t i = 0; i < next.size(); ++i)
            next[i]->start();
        if (!next.empty())
            return;
    }
    else if (routingKey != "parallel" && nextToStart < children.size())
    {
        // Fallback: simple sequential order.
        children[nextToStart++]->start();
        return;
    }
    if (isComplete() && parent)
        parent->onChildCompleted(*this);
}

bool WorkContainer::isComplete() const
{
    if (routing)
        return routing->isComplete(*this);
    for (std::size_t i = 0; i < children.size(); ++i)
        if (!children[i]->isComplete())
            return false;
    return true;
}

void WorkContainer::setRouting(std::shared_ptr<RoutingStrategy> r) { routing = r; }
std::shared_ptr<RoutingStrategy> WorkContainer::getRouting() const { return routing; }

void WorkContainer::setRoutingKey(const std::string &key) { routingKey = key; }
const std::string &WorkContainer::getRoutingKey() const { return routingKey; }

const WorkflowData &WorkContainer::routingData() const
{
    static const WorkflowData none;
    return none;
}

WorkNodeIterator *WorkContainer::createLevelOrderIterator()
{
    return new LevelOrderIterator(this); // INTEGRATION: Person 7 ctor(WorkNode*)
}

WorkNode *WorkContainer::findById(const std::string &nodeId)
{
    if (id == nodeId)
        return this;
    for (std::size_t i = 0; i < children.size(); ++i)
    {
        WorkNode *found = children[i]->findById(nodeId);
        if (found)
            return found;
    }
    return nullptr;
}
