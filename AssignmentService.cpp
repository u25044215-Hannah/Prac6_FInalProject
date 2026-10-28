#include "AssignmentService.h"
#include "ParticipantDirectory.h"
#include <stdexcept>

AssignmentService::AssignmentService(ParticipantDirectory* directory)
    : directory_(directory) {
    if (!directory_) throw std::invalid_argument("AssignmentService requires a directory");
}
Participant* AssignmentService::assign(Task& item) {
    (void)item;
    // Task.h must expose its assignment strategy and eligibility requirements.
    // Do not select an arbitrary participant while that contract is undefined.
    throw std::logic_error("AssignmentService::assign requires Task.h and its assignment policy interface");
}
