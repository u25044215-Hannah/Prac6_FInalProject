/**
 * @file RecoveryManager.cpp
 * @author Janke Rall, u24571238 (Commands, History & Recovery)
 */

#include "RecoveryManager.h"

#include <memory>
#include <stdexcept>
#include <utility>

#include "ActionHistory.h"
#include "InstanceSnapshot.h"
#include "WorkflowInstance.h"

RecoveryManager::RecoveryManager() {}

RecoveryManager::~RecoveryManager() {
    // The caretaker owns every snapshot it stores (composition in the UML).
    for (std::map<std::string, InstanceSnapshot*>::iterator it = checkpoints.begin();
         it != checkpoints.end(); ++it) {
        delete it->second;
    }
}

void RecoveryManager::checkpoint(WorkflowInstance& inst, const std::string& label) {
    // The guard deletes the snapshot if anything below throws.
    std::unique_ptr<InstanceSnapshot> snapshot(inst.createSnapshot());
    if (!snapshot) {
        throw std::runtime_error("RecoveryManager::checkpoint: createSnapshot() returned null");
    }

    // insert() does nothing if the key already exists, and tells us where the entry is.
    std::pair<std::map<std::string, InstanceSnapshot*>::iterator, bool> slot =
        checkpoints.insert(std::make_pair(key(inst, label), static_cast<InstanceSnapshot*>(nullptr)));

    delete slot.first->second; // replace an older checkpoint with the same label
    slot.first->second = snapshot.release(); // the map owns the new one now

    inst.getHistory().appendNote("Checkpoint '" + label + "'");
}

void RecoveryManager::rollback(WorkflowInstance& inst, const std::string& label) {
    std::map<std::string, InstanceSnapshot*>::iterator it = checkpoints.find(key(inst, label));
    if (it == checkpoints.end()) {
        throw std::out_of_range("No checkpoint '" + label + "' for instance " + inst.getId());
    }
    inst.restore(*it->second); // only the originator can read the snapshot
    // The checkpoint is kept, so the same point can be rolled back to again.
    inst.getHistory().appendNote("Rolled back to '" + label + "'");
}

void RecoveryManager::discard(const WorkflowInstance& inst, const std::string& label) {
    std::map<std::string, InstanceSnapshot*>::iterator it = checkpoints.find(key(inst, label));
    if (it != checkpoints.end()) {
        delete it->second;
        checkpoints.erase(it);
    }
}

void RecoveryManager::discardAll(const WorkflowInstance& inst) {
    const std::string prefix = inst.getId() + "/";
    std::map<std::string, InstanceSnapshot*>::iterator it = checkpoints.begin();
    while (it != checkpoints.end()) {
        if (it->first.compare(0, prefix.size(), prefix) == 0) {
            delete it->second;
            checkpoints.erase(it++); // post-increment: move on before erasing
        } else {
            ++it;
        }
    }
}

bool RecoveryManager::hasCheckpoint(const WorkflowInstance& inst, const std::string& label) const {
    return checkpoints.count(key(inst, label)) > 0;
}

std::size_t RecoveryManager::count() const {
    return checkpoints.size();
}

std::string RecoveryManager::key(const WorkflowInstance& inst, const std::string& label) {
    // Assumes instance ids contain no '/', which holds for engine-generated ids.
    return inst.getId() + "/" + label;
}
