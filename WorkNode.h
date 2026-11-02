/**
 * @file WorkNode.h
 * @brief Declares the project interfaces and types.
 */

#ifndef WORKNODE_H
#define WORKNODE_H

#include <map>
#include <string>

class WorkContainer;
class WorkNodeVisitor;
class WorkNodeIterator;

/**
 * @brief Represents the WorkNode component of the workflow engine.
 */
class WorkNode {
public:
    /**
     * @brief Performs the WorkNode operation.
     */
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
    /**
     * @brief Performs the createReadyIterator operation.
     * @return The result of the operation.
     */
    virtual WorkNodeIterator* createReadyIterator() = 0;

    /**
     * @brief Find a node by id in this subtree.
     * Default: returns this if the id matches, otherwise nullptr.
     * WorkContainer overrides it to search its children.
     */
    virtual WorkNode* findById(const std::string& nodeId);

    /// Memento support: record / re-apply the state name of every work item in
    /// this subtree, keyed by node id. Nodes with no state of their own do nothing.
    virtual void captureState(std::map<std::string, std::string>& out) const { (void)out; }
    /**
     * @brief Performs the restoreState operation.
     * @param in Parameter used by this operation.
     */
    virtual void restoreState(const std::map<std::string, std::string>& in) { (void)in; }

    /**
     * @brief Performs the getId operation.
     * @return The result of the operation.
     */
    const std::string& getId() const;
    /**
     * @brief Performs the getName operation.
     * @return The result of the operation.
     */
    const std::string& getName() const;
    /**
     * @brief Performs the getParent operation.
     * @return The result of the operation.
     */
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