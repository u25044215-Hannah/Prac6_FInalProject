#ifndef WORKCONTAINER_H
#define WORKCONTAINER_H

#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include "WorkNode.h"

class RoutingStrategy;   // Person 5
class WorkflowData;      // Person 1

class WorkContainer : public WorkNode {
public:
    virtual ~WorkContainer();

    /// Take ownership of n and set its parent to this. n must have no parent.
    void add(WorkNode* n);

    /// Detach n from this container and delete it. No-op if n is not a child.
    void remove(WorkNode* n);

    /**
     * @brief Put `replacement` in the slot of `oldNode` WITHOUT deleting oldNode.
     * Used by the builder to wrap a task in decorators: the decorator takes
     * over ownership of oldNode. Throws std::logic_error if oldNode is not a child.
     */
    void replaceChild(WorkNode* oldNode, WorkNode* replacement);

    const std::vector<WorkNode*>& getChildren() const;

    /// Called by a child when it completes: asks the routing strategy (or the
    /// key-based fallback) what to start next, then tells this container's
    /// own parent once everything is done.
    virtual void onChildCompleted(WorkNode& child);

    /// Complete according to the routing strategy; without one, when all
    /// children are complete (vacuously true when empty).
    virtual bool isComplete() const;

    // --- Strategy ---
    /// Shared with clones (strategies are configuration, not per-run state).
    void setRouting(std::shared_ptr<RoutingStrategy> r);
    std::shared_ptr<RoutingStrategy> getRouting() const;

    // --- Routing label from the builder ---
    void setRoutingKey(const std::string& key);
    const std::string& getRoutingKey() const;

    // --- Iterator ---
    /// Breadth-first iterator over this subtree. Caller owns the result.
    WorkNodeIterator* createLevelOrderIterator();

    /// Searches this container and all descendants.
    virtual WorkNode* findById(const std::string& nodeId);

protected:
    WorkContainer(const std::string& id, const std::string& name);

    /// Deep copy: clones every child, re-parents the copies, shares routing.
    WorkContainer(const WorkContainer& other);

    /// Start the first child (sequential) or all children (parallel).
    void startChildren();

    /// Data handed to the routing strategy. Default: empty. SubProcess
    /// overrides it with its local data.
    virtual const WorkflowData& routingData() const;

    std::vector<WorkNode*> children;
    std::string routingKey;
    std::shared_ptr<RoutingStrategy> routing;
    std::size_t nextToStart;   ///< key-based sequential progress
};

#endif // WORKCONTAINER_H
