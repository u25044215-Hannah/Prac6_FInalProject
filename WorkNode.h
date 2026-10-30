#ifndef WORKNODE_H
#define WORKNODE_H

#include <string>

class WorkContainer;
class WorkNodeVisitor;
class WorkNodeIterator;

class WorkNode {
public:
    virtual ~WorkNode();

    /// Begin execution of this node.
    virtual void start() = 0;
    /// Cancel this node (and anything below it).
    virtual void cancel() = 0;
    /// True when this node has finished.
    virtual bool isComplete() const = 0;

    /// Prototype: deep copy. The copy has no parent and shares no mutable state.
    virtual WorkNode* clone() const = 0;

    /// Visitor: double dispatch. Containers visit themselves, then their
    /// children, so a visitor sees every node exactly once.
    virtual void accept(WorkNodeVisitor& v) = 0;

    /// Iterator factories. The caller owns (and deletes) the returned iterator.
    virtual WorkNodeIterator* createIterator() = 0;
    virtual WorkNodeIterator* createReadyIterator() = 0;

    /**
     * @brief Find a node by id in this subtree.
     * Default: returns this if the id matches, otherwise nullptr.
     * WorkContainer overrides it to search its children.
     */
    virtual WorkNode* findById(const std::string& nodeId);

    const std::string& getId() const;
    const std::string& getName() const;
    WorkContainer* getParent() const;

    /// Called by WorkContainer::add / remove only.
    void setParent(WorkContainer* p);

protected:
    WorkNode(const std::string& id, const std::string& name);

    /// Copy used by clone(): copies id and name, parent becomes nullptr.
    WorkNode(const WorkNode& other);

    std::string id;
    std::string name;
    WorkContainer* parent;

private:
    WorkNode& operator=(const WorkNode&) = delete;
};

#endif // WORKNODE_H