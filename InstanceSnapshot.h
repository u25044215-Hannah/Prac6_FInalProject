#ifndef INSTANCESNAPSHOT_H
#define INSTANCESNAPSHOT_H

#include <string>
#include <iostream>
#include <map>
#include <vector>

class InstanceSnapshot {
private:
    std::map<std::string, std::string> itemStates;
    std::map<std::string, std::string> assignees;
    WorkflowData data;
    std::vector<Deadline> timers;

    InstanceSnapshot();
};

#endif 