#ifndef RECOVERYMANAGER_H
#define RECOVERTMANAGER_H

#include <string>
#include <iostream>
#include <map>

class InstanceSnapshot {
private:
    std::map<std::string, InstanceSnapshot*> checkpoints;
public:
    void checkpoint(WorkflowInstance& inst, std::string label);
    void rollback(WorkflowInstance& inst, std::string label);
    void discard(std::string label);
};

#endif