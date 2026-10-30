#ifndef DEFINITIONFILEREADER_H
#define DEFINITIONFILEREADER_H

#include <string>
#include <vector>
#include "WorkflowDirector.h"

class DefinitionFileReader : public WorkflowDirector {
public:
    explicit DefinitionFileReader(const std::string& path);
    virtual ~DefinitionFileReader();

    /// Throws std::runtime_error if the file cannot be opened or a line is
    /// malformed (message includes the line number).
    virtual void construct(WorkflowDefinitionBuilder& b);

private:
    /// Split a line into whitespace separated tokens (comments removed).
    static std::vector<std::string> tokenize(const std::string& line);

    /// Map one tokenised line onto a builder call.
    void dispatch(WorkflowDefinitionBuilder& b,
                  const std::vector<std::string>& tokens,
                  int lineNumber) const;

    std::string path;
};

#endif