#ifndef DEFINITIONCOMMANDS_H
#define DEFINITIONCOMMANDS_H

#include <string>
#include <vector>

class WorkflowDefinitionBuilder;

void applyDefinitionCommand(WorkflowDefinitionBuilder& b,
                            const std::string& command,
                            const std::vector<std::string>& args);

#endif // DEFINITIONCOMMANDS_H