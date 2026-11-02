/**
 * @file ApprovalLevel.h
 * @brief Declares the project interfaces and types.
 */

#ifndef APPROVAL_LEVEL_H
#define APPROVAL_LEVEL_H

#include <string>
#include "ApprovalHandler.h"
#include "ApprovalLimits.h"

/**
 * @brief Represents the ApprovalLevel component of the workflow engine.
 */
class ApprovalLevel : public ApprovalHandler {
private:
    std::string role_;
    ApprovalLimits limits_; // UML shows an ApprovalLimits value, not a pointer
public:
    /**
     * @brief Performs the ApprovalLevel operation.
     * @param role Parameter used by this operation.
     * @param limits Parameter used by this operation.
     */
    ApprovalLevel(const std::string& role, const ApprovalLimits& limits);
    // Compatibility overload for existing teammates calling with a pointer.
    /**
     * @brief Performs the ApprovalLevel operation.
     * @param role Parameter used by this operation.
     * @param limits Parameter used by this operation.
     */
    ApprovalLevel(const std::string& role, const ApprovalLimits* limits);
    /**
     * @brief Performs the handle operation.
     * @param request Parameter used by this operation.
     * @return The result of the operation.
     */
    bool handle(ApprovalRequest& request) override;
};

#endif // APPROVAL_LEVEL_H

