#include "InteractiveDefinitionDirector.h"

#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "DefinitionCommands.h"
#include "WorkflowDefinitionBuilder.h"

namespace
{

    // The keys each command needs, in the order the shared command parser expects.
    std::vector<std::string> requiredKeys(const std::string &cmd)
    {
        if (cmd == "definition" || cmd == "stage" || cmd == "subprocess")
            return {"name"};
        if (cmd == "end")
            return {};
        if (cmd == "human")
            return {"id", "role"};
        if (cmd == "automated")
            return {"id", "connector"};
        if (cmd == "depends")
            return {"id", "prereq"};
        if (cmd == "routing")
            return {"key"};
        if (cmd == "assign")
            return {"id", "key"};
        if (cmd == "deadline")
            return {"id", "seconds"};
        if (cmd == "decorate")
            return {"id", "key"};
        if (cmd == "escalation")
            return {"role", "seconds"};
        if (cmd == "approval")
            return {"role", "maxPriority"}; // "categories" is optional
        throw std::runtime_error("unknown command '" + cmd + "'");
    }

    std::string lower(std::string s)
    {
        for (std::size_t i = 0; i < s.size(); ++i)
            if (s[i] >= 'A' && s[i] <= 'Z')
                s[i] = static_cast<char>(s[i] - 'A' + 'a');
        return s;
    }

} // namespace

InteractiveDefinitionDirector::InteractiveDefinitionDirector() : steps() {}

InteractiveDefinitionDirector::~InteractiveDefinitionDirector() {}

void InteractiveDefinitionDirector::addStep(const WorkflowData &step) { steps.push_back(step); }

void InteractiveDefinitionDirector::clear() { steps.clear(); }

std::size_t InteractiveDefinitionDirector::stepCount() const { return steps.size(); }

void InteractiveDefinitionDirector::construct(WorkflowDefinitionBuilder &b)
{
    for (std::size_t i = 0; i < steps.size(); ++i)
    {
        const WorkflowData &step = steps[i];
        try
        {
            if (!step.has("cmd"))
                throw std::runtime_error("missing 'cmd'");
            const std::string cmd = lower(step.get("cmd"));

            std::vector<std::string> args;
            const std::vector<std::string> keys = requiredKeys(cmd);
            for (std::size_t k = 0; k < keys.size(); ++k)
            {
                if (!step.has(keys[k]))
                    throw std::runtime_error("missing '" + keys[k] + "' for command '" + cmd + "'");
                args.push_back(step.get(keys[k]));
            }
            if (cmd == "approval" && step.has("categories"))
                args.push_back(step.get("categories"));

            applyDefinitionCommand(b, cmd, args);
        }
        catch (const std::exception &e)
        {
            std::ostringstream msg;
            msg << "step " << (i + 1) << ": " << e.what();
            throw std::runtime_error(msg.str());
        }
    }
    // build() is NOT called here: the caller (TividyEngine) does that.
}