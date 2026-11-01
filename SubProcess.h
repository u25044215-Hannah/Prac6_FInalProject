/**
 * @file SubProcess.h
 * @brief Declares the project interfaces and types.
 */

#ifndef SUBPROCESS_H
#define SUBPROCESS_H

#include "WorkContainer.h"
#include "WorkflowData.h"      // Person 1
#include "WorkflowOutcome.h"   // Person 1

/**
 * @brief Represents the SubProcess component of the workflow engine.
 */
class SubProcess : public WorkContainer {
public:
    /**
     * @brief Performs the SubProcess operation.
     * @param id Parameter used by this operation.
     * @param name Parameter used by this operation.
     */
    SubProcess(const std::string& id, const std::string& name);
    /**
     * @brief Performs the SubProcess operation.
     */
    virtual ~SubProcess();

    /**
     * @brief Performs the start operation.
     */
    virtual void start();
    /**
     * @brief Performs the cancel operation.
     */
    virtual void cancel();
    /**
     * @brief Performs the isComplete operation.
     * @return The result of the operation.
     */
    virtual bool isComplete() const;

    /// Marks the outcome COMPLETED when the last child finishes, then bubbles up.
    virtual void onChildCompleted(WorkNode& child);

    /**
     * @brief Performs the clone operation.
     * @return The result of the operation.
     */
    virtual SubProcess* clone() const;

    /// Calls v.visit(*this), then accept(v) on every child.
    virtual void accept(WorkNodeVisitor& v);

    /**
     * @brief Performs the createIterator operation.
     * @return The result of the operation.
     */
    virtual WorkNodeIterator* createIterator();        ///< depth-first
    /**
     * @brief Performs the createReadyIterator operation.
     * @return The result of the operation.
     */
    virtual WorkNodeIterator* createReadyIterator();   ///< only ready work items

    /**
     * @brief Performs the getLocalData operation.
     * @return The result of the operation.
     */
    WorkflowData& getLocalData();
    /**
     * @brief Performs the getLocalData operation.
     * @return The result of the operation.
     */
    const WorkflowData& getLocalData() const;
    /**
     * @brief Performs the getOutcome operation.
     * @return The result of the operation.
     */
    WorkflowOutcome getOutcome() const;
    /**
     * @brief Performs the setOutcome operation.
     * @param o Parameter used by this operation.
     */
    void setOutcome(WorkflowOutcome o);

protected:
    /// Deep copy: copies localData and outcome as well as the children.
    SubProcess(const SubProcess& other);

    /// Routing decisions inside a sub-process use its own local data.
    virtual const WorkflowData& routingData() const;

private:
    WorkflowData localData;
    WorkflowOutcome outcome;
};

#endif // SUBPROCESS_H