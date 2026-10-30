/**
 * @file InstanceSnapshot.cpp
 * @author Janke Rall, u24571238 (Commands, History & Recovery)
 *
 * Memento only holds state. WorkflowInstance (Person 1) fills the fields in createSnapshot() and reads them in restore().
 */

#include "InstanceSnapshot.h"

InstanceSnapshot::InstanceSnapshot() : createdAt(std::time(nullptr)) {}

InstanceSnapshot::~InstanceSnapshot() {}

std::time_t InstanceSnapshot::takenAt() const {
    return createdAt;
}
