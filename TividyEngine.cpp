#include "TividyEngine.h"
#include "InMemoryDefinitionRepository.h"
#include "TividyException.h"

TividyEngine::TividyEngine() : definitions_(new InMemoryDefinitionRepository()), versions_(instances_, recovery_) {}

TividyEngine::TividyEngine(std::unique_ptr<DefinitionRepository> repository) : definitions_(std::move(repository)), versions_(instances_, recovery_) {
    if (!definitions_) 
    {
        throw InvalidOperationException("TividyEngine needs a definition repository");
    }
}

const WorkflowDefinition* TividyEngine::publishDefinition(std::unique_ptr<WorkflowDefinition> def) {
    const WorkflowDefinition* stored = definitions_->save(std::move(def));
    versions_.onNewVersion(*stored);
    return stored;
}

WorkflowInstance* TividyEngine::startWorkflow(const std::string& name, const WorkflowData& input) {
    return startWorkflow(name, 0, input);
}

WorkflowInstance* TividyEngine::startWorkflow(const std::string& name, int version, const WorkflowData& input) {
    const WorkflowDefinition* def = versions_.resolve(*definitions_, name, version);
    WorkflowInstance* instance = instances_.createInstance(*def, input);
    instance->start();
    return instance;
}

void TividyEngine::submit(std::unique_ptr<WorkflowCommand> cmd, const std::string& instanceId) {
    if (!cmd) 
    {
        throw InvalidOperationException("Cannot submit a null command");
    }
    WorkflowInstance& instance = instances_.get(instanceId);
    if (instance.isFinished()) 
    {
        throw InvalidOperationException("Instance " + instanceId + " is already finished (" + outcomeToString(instance.getOutcome()) + ")");
    }
    commands_.submit(cmd.release(), instance);
}