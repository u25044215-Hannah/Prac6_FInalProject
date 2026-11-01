<<<<<<< Updated upstream
/**
 * @file WorkflowInstance.h
 * @brief One independent execution of a workflow definition
 */
#ifndef TIVIDY_WORKFLOW_INSTANCE_H
#define TIVIDY_WORKFLOW_INSTANCE_H

#include "ActionHistory.h"
#include "WorkflowData.h"
#include "WorkflowOutcome.h"
#include <memory>
#include <string>

class WorkNode;
class WorkflowDefinition;
class InstanceSnapshot;

/**
 * @brief A running process created from a WorkflowDefinition
 *
 * Owns a deep clone of the definition's tree, its own data and its own history,
 * so it never affects other instances. Only points (non-owning) at its definition.
 */
class WorkflowInstance {
public:
    /// @throws InvalidOperationException if @p id is empty or cloning fails
    WorkflowInstance(const std::string& id, const WorkflowDefinition& def, const WorkflowData& input);
    ~WorkflowInstance();

    const std::string& getId() const { return id_; }
    const WorkflowDefinition& getDefinition() const { return *definition_; }
    std::string getDefinitionName() const;
    int getDefinitionVersion() const;

    /// @throws InvalidOperationException if already started or finished
    void start();
    bool isStarted() const { return started_; }

    ///instance's own work tree (not the definition's template)
    WorkNode& getRoot() { return *root_; }
    const WorkNode& getRoot() const { return *root_; }
    WorkflowData& getData() { return data_; }
    const WorkflowData& getData() const { return data_; }
    ActionHistory& getHistory() { return history_; }
    const ActionHistory& getHistory() const { return history_; }

    WorkflowOutcome getOutcome() const { return outcome_; }
    bool isFinished() const { return isTerminal(outcome_); }

    /// @throws InvalidOperationException if a different final outcome is already set
    void setOutcome(WorkflowOutcome outcome);

    ///point to another version of the same process
    void rebindTo(const WorkflowDefinition& def);

    ///Memento
    std::unique_ptr<InstanceSnapshot> createSnapshot() const;
    void restore(const InstanceSnapshot& snapshot);

private:
    WorkflowInstance(const WorkflowInstance&);
    WorkflowInstance& operator=(const WorkflowInstance&);

    std::string id_;
    const WorkflowDefinition* definition_;
    std::unique_ptr<WorkNode> root_;
    WorkflowData data_;
    ActionHistory history_;
    WorkflowOutcome outcome_;
    bool started_;
};

=======
/**
 * @file WorkflowInstance.h
 * @brief One independent execution of a workflow definition
 */
#ifndef TIVIDY_WORKFLOW_INSTANCE_H
#define TIVIDY_WORKFLOW_INSTANCE_H

#include "ActionHistory.h"
#include "WorkflowData.h"
#include "WorkflowOutcome.h"

// Observer Pattern
#include "include/WorkflowEventPublisher.h"
#include "include/DependencyResolver.h"

#include <memory>
#include <string>

class WorkNode;
class WorkflowDefinition;
class InstanceSnapshot;

/**
 * @brief A running process created from a WorkflowDefinition
 *
 * Owns a deep clone of the definition's tree, its own data and its own history,
 * so it never affects other instances. Only points (non-owning) at its definition.
 *
 * @details
 * Each workflow instance also owns an event publisher and a dependency resolver.
 * Tasks publish state-change events through the publisher, allowing the
 * dependency resolver to react when prerequisite tasks are completed.
 *
 * This integrates the Prototype, State and Observer design patterns.
 */
class WorkflowInstance {
public:

    /// @throws InvalidOperationException if @p id is empty or cloning fails
    WorkflowInstance(
        const std::string& id,
        const WorkflowDefinition& def,
        const WorkflowData& input
    );

    ~WorkflowInstance();

    const std::string& getId() const { return id_; }

    const WorkflowDefinition& getDefinition() const {
        return *definition_;
    }

    std::string getDefinitionName() const;
    int getDefinitionVersion() const;

    /// @throws InvalidOperationException if already started or finished
    void start();

    bool isStarted() const { return started_; }

    /// Instance's own work tree (not the definition's template)
    WorkNode& getRoot() { return *root_; }

    const WorkNode& getRoot() const { return *root_; }

    WorkflowData& getData() { return data_; }

    const WorkflowData& getData() const { return data_; }

    ActionHistory& getHistory() { return history_; }

    const ActionHistory& getHistory() const {
        return history_;
    }

    WorkflowOutcome getOutcome() const { return outcome_; }

    bool isFinished() const { return isTerminal(outcome_); }

    /// @throws InvalidOperationException if a different final outcome is already set
    void setOutcome(WorkflowOutcome outcome);

    /// Point to another version of the same process
    void rebindTo(const WorkflowDefinition& def);

    /// Memento
    std::unique_ptr<InstanceSnapshot> createSnapshot() const;

    void restore(const InstanceSnapshot& snapshot);

    /**
     * @brief Returns this workflow instance's event publisher.
     *
     * @return Reference to the publisher owned by this instance.
     *
     * @details
     * The publisher notifies registered observers when workflow
     * events occur, including task state changes.
     */
    WorkflowEventPublisher& getEventPublisher() {
        return eventPublisher_;
    }

private:

    WorkflowInstance(const WorkflowInstance&);
    WorkflowInstance& operator=(const WorkflowInstance&);

    std::string id_;

    const WorkflowDefinition* definition_;

    std::unique_ptr<WorkNode> root_;

    WorkflowData data_;

    ActionHistory history_;

    WorkflowOutcome outcome_;

    bool started_;

    /**
     * @brief Publisher responsible for notifying workflow observers.
     *
     * Each workflow instance owns a separate publisher to prevent
     * events from different instances interfering with each other.
     */
    WorkflowEventPublisher eventPublisher_;

    /**
     * @brief Observer that checks and unlocks dependent tasks.
     *
     * Owned by the workflow instance and subscribed to
     * WORK_ITEM_STATE_CHANGED events.
     */
    std::unique_ptr<DependencyResolver> dependencyResolver_;

    /**
     * @brief Connects every cloned task to this instance's publisher.
     *
     * @details
     * Uses depth-first traversal to visit all tasks, including those
     * nested inside stages and subprocesses.
     *
     * Each task receives the publisher and workflow instance ID
     * required to publish correctly identified events.
     */
    void connectTaskObservers();
};

>>>>>>> Stashed changes
#endif // TIVIDY_WORKFLOW_INSTANCE_H