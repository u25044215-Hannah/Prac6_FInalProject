/**
 * @file InstanceSnapshot.h
 * @brief Opaque saved state of one workflow instance.
 * @author Janke Rall, u24571238 (Commands, History & Recovery)
 *
 * Pattern: Memento -> Memento.
 */

#ifndef TIVIDY_INSTANCESNAPSHOT_H
#define TIVIDY_INSTANCESNAPSHOT_H

#include <ctime>
#include <map>
#include <string>
#include <vector>
#include "WorkflowData.h"
#include "Deadline.h"

/**
 * @brief The state of a WorkflowInstance at a checkpoint.
 *
 * It has two interfaces, as in GoF:
 *  - a wide one (all the private fields) that only the originator, WorkflowInstance, can use - it is a friend and the constructor is private, so only 
 *    WorkflowInstance::createSnapshot() can create one;
 *  - a narrow one (takenAt()) that the caretaker, RecoveryManager, may use without being able to read or change the saved state.
 *
 * What is saved: the state name and assignee of every work item (keyed by node id), the instance's WorkflowData, and the running timers.
 * What is NOT saved: the ActionHistory. History is append-only, so a rollback never erases the record of what happened.
 */
class InstanceSnapshot {
    friend class WorkflowInstance;

public:
    ~InstanceSnapshot();

    /** @brief When the snapshot was taken (narrow interface for the caretaker). */
    std::time_t takenAt() const;

private:
    /** @brief Private: only WorkflowInstance may create a snapshot. */
    InstanceSnapshot();

    std::map<std::string, std::string> itemStates;  // node id -> state name, e.g. "InProgress".
    std::map<std::string, std::string> assignees;   // node id -> participant id ("" = unassigned).
    WorkflowData data;                              // Copy of the instance's data.
    std::vector<Deadline> timers;                   // Timers running at the checkpoint.
    std::time_t createdAt;

    InstanceSnapshot(const InstanceSnapshot&) = delete;
    InstanceSnapshot& operator=(const InstanceSnapshot&) = delete;
};

#endif 
