/**
 * @file WorkItem.h
 * @brief Declares the project interfaces and types.
 */

#ifndef WORKITEM_H
#define WORKITEM_H

#include <string>
#include <utility>
#include "WorkflowData.h"
#include "WorkNode.h"
#include "Participant.h"
#include "WorkflowData.h"

/**
 * @brief Represents the WorkItem component of the workflow engine.
 */
class WorkItem : public WorkNode {
public:
    /**
     * @brief Performs the WorkItem operation.
     * @param id Parameter used by this operation.
     * @param name Parameter used by this operation.
     */
    WorkItem(std::string id, std::string name) : WorkNode(std::move(id), std::move(name)) {}
    /**
     * @brief Performs the WorkItem operation.
     */
    ~WorkItem() override = default;

    /**
     * @brief Performs the makeAvailable operation.
     */
    virtual void makeAvailable() = 0;
    /**
     * @brief Performs the assign operation.
     * @param p Parameter used by this operation.
     * @param actor Parameter used by this operation.
     */
    virtual void assign(Participant& p, Participant& actor) = 0;
    /**
     * @brief Performs the acceptAssignment operation.
     * @param actor Parameter used by this operation.
     */
    virtual void acceptAssignment(Participant& actor) = 0;
    /**
     * @brief Performs the declineAssignment operation.
     * @param actor Parameter used by this operation.
     */
    virtual void declineAssignment(Participant& actor) = 0;
    /**
     * @brief Performs the complete operation.
     * @param result Parameter used by this operation.
     * @param actor Parameter used by this operation.
     */
    virtual void complete(WorkflowData result, Participant& actor) = 0;
    /**
     * @brief Performs the fail operation.
     * @param reason Parameter used by this operation.
     */
    virtual void fail(const std::string& reason) = 0;
    /**
     * @brief Performs the reject operation.
     * @param reason Parameter used by this operation.
     * @param actor Parameter used by this operation.
     */
    virtual void reject(const std::string& reason, Participant& actor) = 0;
    /**
     * @brief Performs the escalate operation.
     */
    virtual void escalate() = 0;
    /**
     * @brief Performs the hold operation.
     */
    virtual void hold() = 0;
    /**
     * @brief Performs the release operation.
     */
    virtual void release() = 0;
    /**
     * @brief Performs the stateName operation.
     * @return The result of the operation.
     */
    virtual std::string stateName() const = 0;
};

#endif // WORKITEM_H