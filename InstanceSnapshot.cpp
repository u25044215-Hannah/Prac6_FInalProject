/**
 * @file InstanceSnapshot.cpp
 * Memento only holds state. WorkflowInstance fills the fields in createSnapshot() and reads them in restore().
 */

#include "InstanceSnapshot.h"

InstanceSnapshot::InstanceSnapshot()
    : instanceId(), outcome(), started(false), createdAt(std::time(nullptr)) {}

InstanceSnapshot::~InstanceSnapshot() {}

std::time_t InstanceSnapshot::takenAt() const {
    return createdAt;
}
