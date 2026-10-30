#ifndef ASSIGNMENT_SERVICE_H
#define ASSIGNMENT_SERVICE_H
#include "ParticipantDirectory.h"

class AssignmentService {
private:
    ParticipantDirectory* directory_ = nullptr; // non-owning engine service
public:
    explicit AssignmentService(ParticipantDirectory* directory);
    Participant* assign(Task& item);
};

#endif // ASSIGNMENT_SERVICE_H
