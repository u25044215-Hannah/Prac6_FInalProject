#ifndef DEFINITION_COMMANDS_H
#define DEFINITION_COMMANDS_H

#include "DefinitionCommands.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <set>
#include <stdexcept>

#include "WorkflowDefinitionBuilder.h"
#include "ApprovalLimits.h" // Person 5

namespace
{

    std::string lower(std::string s)
    {
        for (std::size_t i = 0; i < s.size(); ++i)
            s[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(s[i])));
        return s;
    }

    std::string join(const std::vector<std::string> &v)
    {
        std::string out;
        for (std::size_t i = 0; i < v.size(); ++i)
        {
            if (i)
                out += " ";
            out += v[i];
        }
        return out;
    }

    void expect(const std::vector<std::string> &a, std::size_t lo, std::size_t hi,
                const std::string &usage)
    {
        if (a.size() < lo || a.size() > hi)
            throw std::runtime_error("expected: " + usage);
    }

    int toInt(const std::string &s, const std::string &what)
    {
        char *end = nullptr;
        long v = std::strtol(s.c_str(), &end, 10);
        if (s.empty() || *end != '\0')
            throw std::runtime_error("'" + s + "' is not a whole number (" + what + ")");
        return static_cast<int>(v);
    }

    // "a,b,c" -> {a,b,c}; "-" means none.
    std::set<std::string> splitCategories(const std::string &s)
    {
        std::set<std::string> out;
        if (s == "-")
            return out;
        std::string cur;
        for (std::size_t i = 0; i <= s.size(); ++i)
        {
            if (i == s.size() || s[i] == ',')
            {
                if (!cur.empty())
                    out.insert(cur);
                cur.clear();
            }
            else
            {
                cur += s[i];
            }
        }
        return out;
    }

} // namespace

inline void applyDefinitionCommand(WorkflowDefinitionBuilder &b,
                            const std::string &command,
                            const std::vector<std::string> &args)
{
    const std::string c = lower(command);

    if (c == "definition")
    {
        if (args.empty())
            throw std::runtime_error("expected: definition <name>");
        b.beginDefinition(join(args));
    }
    else if (c == "stage")
    {
        if (args.empty())
            throw std::runtime_error("expected: stage <name>");
        b.beginStage(join(args));
    }
    else if (c == "subprocess")
    {
        if (args.empty())
            throw std::runtime_error("expected: subprocess <name>");
        b.beginSubProcess(join(args));
    }
    else if (c == "end")
    {
        expect(args, 0, 0, "end");
        b.endContainer();
    }
    else if (c == "human")
    {
        expect(args, 2, 2, "human <id> <role>");
        b.addHumanTask(args[0], args[1]);
    }
    else if (c == "automated")
    {
        expect(args, 2, 2, "automated <id> <connector>");
        b.addAutomatedTask(args[0], args[1]);
    }
    else if (c == "depends")
    {
        expect(args, 2, 2, "depends <id> <prereqId>");
        b.dependsOn(args[0], args[1]);
    }
    else if (c == "routing")
    {
        expect(args, 1, 1, "routing <key>");
        b.setRouting(args[0]);
    }
    else if (c == "assign")
    {
        expect(args, 2, 2, "assign <id> <key>");
        b.setAssignment(args[0], args[1]);
    }
    else if (c == "deadline")
    {
        expect(args, 2, 2, "deadline <id> <seconds>");
        b.setDeadline(args[0], toInt(args[1], "seconds"));
    }
    else if (c == "decorate")
    {
        expect(args, 2, 2, "decorate <id> <key>");
        b.decorateWith(args[0], args[1]);
    }
    else if (c == "escalation")
    {
        expect(args, 2, 2, "escalation <role> <seconds>");
        b.addEscalationLevel(args[0], toInt(args[1], "seconds"));
    }
    else if (c == "approval")
    {
        expect(args, 2, 3, "approval <role> <maxPriority> [cat,cat,...]");
        std::set<std::string> cats;
        if (args.size() == 3)
            cats = splitCategories(args[2]);
        ApprovalLimits limits(toInt(args[1], "maxPriority"), cats); // INTEGRATION: Person 5 ctor
        b.addApprovalLevel(args[0], limits);
    }
    else
    {
        throw std::runtime_error("unknown command '" + command + "'");
    }
}


#endif // DEFINITION_COMMANDS_H