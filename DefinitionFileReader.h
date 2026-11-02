/**
 * @file DefinitionFileReader.h
 * @brief Declares the project interfaces and types.
 */

#ifndef DEFINITIONFILEREADER_H
#define DEFINITIONFILEREADER_H

#include <string>
#include <vector>
#include "WorkflowDirector.h"

/**
 * @brief Represents the DefinitionFileReader component of the workflow engine.
 */
class DefinitionFileReader : public WorkflowDirector {
public:
    /**
     * @brief Performs the DefinitionFileReader operation.
     * @param path Parameter used by this operation.
     * @return The result of the operation.
     */
    explicit DefinitionFileReader(const std::string& path);
    /**
     * @brief Performs the DefinitionFileReader operation.
     */
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
