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
    /**
     * @brief Performs the WorkflowInstance operation.
     */
    ~WorkflowInstance();

    /**
     * @brief Performs the getId operation.
     * @return The result of the operation.
     */
    const std::string& getId() const { return id_; }
    /**
     * @brief Performs the getDefinition operation.
     * @return The result of the operation.
     */
    const WorkflowDefinition& getDefinition() const { return *definition_; }
    /**
     * @brief Performs the getDefinitionName operation.
     * @return The result of the operation.
     */
    std::string getDefinitionName() const;
    /**
     * @brief Performs the getDefinitionVersion operation.
     * @return The result of the operation.
     */
    int getDefinitionVersion() const;

    /// @throws InvalidOperationException if already started or finished
    void start();
    /**
     * @brief Performs the isStarted operation.
     * @return The result of the operation.
     */
    bool isStarted() const { return started_; }

    ///instance's own work tree (not the definition's template)
    WorkNode& getRoot() { return *root_; }
    /**
     * @brief Performs the getRoot operation.
     * @return The result of the operation.
     */
    const WorkNode& getRoot() const { return *root_; }
    /**
     * @brief Performs the getData operation.
     * @return The result of the operation.
     */
    WorkflowData& getData() { return data_; }
    /**
     * @brief Performs the getData operation.
     * @return The result of the operation.
     */
    const WorkflowData& getData() const { return data_; }
    /**
     * @brief Performs the getHistory operation.
     * @return The result of the operation.
     */
    ActionHistory& getHistory() { return history_; }
    /**
     * @brief Performs the getHistory operation.
     * @return The result of the operation.
     */
    const ActionHistory& getHistory() const { return history_; }

    /**
     * @brief Performs the getOutcome operation.
     * @return The result of the operation.
     */
    WorkflowOutcome getOutcome() const { return outcome_; }
    /**
     * @brief Performs the isFinished operation.
     * @return The result of the operation.
     */
    bool isFinished() const { return isTerminal(outcome_); }

    /// @throws InvalidOperationException if a different final outcome is already set
    void setOutcome(WorkflowOutcome outcome);

    ///point to another version of the same process
    void rebindTo(const WorkflowDefinition& def);

    ///Memento
    std::unique_ptr<InstanceSnapshot> createSnapshot() const;
    /**
     * @brief Performs the restore operation.
     * @param snapshot Parameter used by this operation.
     */
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

#endif // TIVIDY_WORKFLOW_INSTANCE_H