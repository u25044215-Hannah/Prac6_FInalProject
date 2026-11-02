/**
 * @file AssignmentService.h
 * @brief Declares the project interfaces and types.
 */

#ifndef ASSIGNMENT_SERVICE_H
#define ASSIGNMENT_SERVICE_H
#include "ParticipantDirectory.h"
class Task;
/**
 * @brief Represents the AssignmentService component of the workflow engine.
 */
class AssignmentService {
private:
    ParticipantDirectory* directory_ = nullptr; // non-owning engine service
public:
    /**
     * @brief Performs the AssignmentService operation.
     * @param directory Parameter used by this operation.
     * @return The result of the operation.
     */
    explicit AssignmentService(ParticipantDirectory* directory);
    /**
     * @brief Performs the assign operation.
     * @param item Parameter used by this operation.
     * @return The result of the operation.
     */
    Participant* assign(Task& item);
};

#endif // ASSIGNMENT_SERVICE_H
