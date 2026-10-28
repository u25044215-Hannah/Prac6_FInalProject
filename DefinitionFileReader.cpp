#include "DefinitionFileReader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "DefinitionCommands.h"
#include "WorkflowDefinitionBuilder.h"

DefinitionFileReader::DefinitionFileReader(const std::string &path)
    : path(path) {}

DefinitionFileReader::~DefinitionFileReader() {}

void DefinitionFileReader::construct(WorkflowDefinitionBuilder &b)
{
    std::ifstream in(path.c_str());
    if (!in.is_open())
        throw std::runtime_error("Cannot open definition file: " + path);

    std::string line;
    int lineNumber = 0;
    while (std::getline(in, line))
    {
        ++lineNumber;
        std::vector<std::string> tokens = tokenize(line);
        if (tokens.empty())
            continue; // blank line or comment
        dispatch(b, tokens, lineNumber);
    }
    // build() is NOT called here: the caller (TividyEngine) does that.
}

std::vector<std::string> DefinitionFileReader::tokenize(const std::string &line)
{
    std::string text = line;
    std::string::size_type hash = text.find('#');
    if (hash != std::string::npos)
        text.erase(hash);

    std::vector<std::string> tokens;
    std::istringstream words(text);
    std::string w;
    while (words >> w)
        tokens.push_back(w);
    return tokens;
}

void DefinitionFileReader::dispatch(WorkflowDefinitionBuilder &b,
                                    const std::vector<std::string> &tokens,
                                    int lineNumber) const
{
    std::vector<std::string> args(tokens.begin() + 1, tokens.end());
    try
    {
        applyDefinitionCommand(b, tokens[0], args);
    }
    catch (const std::exception &e)
    {
        std::ostringstream msg;
        msg << path << ":" << lineNumber << ": " << e.what();
        throw std::runtime_error(msg.str());
    }
}