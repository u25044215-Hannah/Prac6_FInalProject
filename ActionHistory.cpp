/**
 * @file ActionHistory.cpp
 * @brief Append-only audit trail of one workflow instance.
 * @author Janke Rall, u24571238 (Commands, History & Recovery)
 */

#include "ActionHistory.h"

#include <ctime>
#include <stdexcept>
#include <utility>

#include "WorkflowCommand.h"

namespace {

/** @brief Formats a time as "HH:MM:SS" for the readable log. */
std::string clockTime(std::time_t t) {
    char buffer[16];
    const std::tm* local = std::localtime(&t);
    if (local == nullptr || std::strftime(buffer, sizeof buffer, "%H:%M:%S", local) == 0) {
        return "--:--:--";
    }
    return buffer;
}

} // namespace

ActionHistory::ActionHistory() {}

ActionHistory::~ActionHistory() {
    // The history owns every command it recorded (composition in the UML).
    for (std::size_t i = 0; i < commands.size(); ++i) {
        delete commands[i];
    }
}

void ActionHistory::append(WorkflowCommand* cmd) {
    if (cmd == nullptr) {
        throw std::invalid_argument("ActionHistory::append: command is null");
    }
    std::string line = clockTime(cmd->getTimestamp()) + "  " + cmd->describe();

    // Make room first, so the two push_backs below cannot fail. If anything throws before this point, we have NOT taken ownership of cmd and the caller (CommandProcessor)
    // still deletes it - no leak, no double delete.
    commands.reserve(commands.size() + 1);
    lines.reserve(lines.size() + 1);

    commands.push_back(cmd);
    lines.push_back(std::move(line));
}

void ActionHistory::appendNote(const std::string& text) {
    lines.push_back(clockTime(std::time(nullptr)) + "  " + text);
}

const std::vector<WorkflowCommand*>& ActionHistory::entries() const {
    return commands;
}

const std::vector<std::string>& ActionHistory::log() const {
    return lines;
}

std::size_t ActionHistory::size() const {
    return commands.size();
}

bool ActionHistory::empty() const {
    return commands.empty();
}
