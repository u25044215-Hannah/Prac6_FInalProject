#pragma once
#include "Person5Forward.h"

class AssignmentService {
private:
    ParticipantDirectory* directory_ = nullptr; // non-owning engine service
public:
    explicit AssignmentService(ParticipantDirectory* directory);
    Participant* assign(Task& item);
};
