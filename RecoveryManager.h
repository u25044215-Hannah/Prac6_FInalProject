/**
 * @file RecoveryManager.h
 * @brief Stores named checkpoints of workflow instances and rolls back to them.
 * @author Janke Rall, u24571238 (Commands, History & Recovery)
 *
 * Pattern: Memento -> Caretaker.
 */

#ifndef TIVIDY_RECOVERYMANAGER_H
#define TIVIDY_RECOVERYMANAGER_H

#include <cstddef>
#include <map>
#include <string>

class InstanceSnapshot;
class WorkflowInstance;

/**
 * @brief Keeps checkpoints before risky steps and restores them on failure.
 *
 * Command undo reverses ONE action. RecoveryManager restores a whole group of changes at once, e.g. "everything since the batch started" or "everything 
 * before migrating to a new definition version" (used by BackwardRecovery).
 *
 * It never reads a snapshot's contents: it only stores it, hands it back to the instance (WorkflowInstance::restore) and deletes it.
 *
 * Checkpoints are keyed by instance id + label, so two instances can both use a label such as "before-run" without clashing.
 *
 * TividyEngine owns exactly one RecoveryManager.
 */
class RecoveryManager {
public:
    RecoveryManager();

    /** @brief Deletes every stored snapshot. */
    ~RecoveryManager();

    /**
     * @brief Saves the instance's current state under a label.
     *
     * An existing checkpoint with the same instance and label is replaced (the old snapshot is deleted). A "Checkpoint '<label>'" note is appended to the
     * instance's history.
     */
    void checkpoint(WorkflowInstance& inst, const std::string& label);

    /**
     * @brief Restores the instance to a saved checkpoint.
     *
     * The checkpoint is kept, so the same point can be rolled back to again. A "Rolled back to '<label>'" note is appended to the history. Throws 
     * std::out_of_range if no such checkpoint exists.
     */
    void rollback(WorkflowInstance& inst, const std::string& label);

    /** @brief Deletes a checkpoint that is no longer needed. Does nothing if absent. */
    void discard(const WorkflowInstance& inst, const std::string& label);

    /** @brief Deletes every checkpoint of an instance (call when it reaches an outcome). */
    void discardAll(const WorkflowInstance& inst);

    /** @brief True if the checkpoint exists. */
    bool hasCheckpoint(const WorkflowInstance& inst, const std::string& label) const;

    /** @brief Total number of stored checkpoints (useful in tests to check for leaks). */
    std::size_t count() const;

private:
    /** @brief Builds the map key "<instanceId>/<label>". */
    static std::string key(const WorkflowInstance& inst, const std::string& label);

    std::map<std::string, InstanceSnapshot*> checkpoints;  // Owned.

    RecoveryManager(const RecoveryManager&) = delete;
    RecoveryManager& operator=(const RecoveryManager&) = delete;
};

#endif 
