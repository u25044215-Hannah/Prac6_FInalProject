/**
 * @file Stage.h
 * @brief Declares the project interfaces and types.
 */

#ifndef STAGE_H
#define STAGE_H

#include "WorkContainer.h"

/**
 * @brief Represents the Stage component of the workflow engine.
 */
class Stage : public WorkContainer {
public:
    /**
     * @brief Performs the Stage operation.
     * @param id Parameter used by this operation.
     * @param name Parameter used by this operation.
     */
    Stage(const std::string& id, const std::string& name);
    /**
     * @brief Performs the Stage operation.
     */
    virtual ~Stage();

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

    /// Deep clone (covariant return).
    virtual Stage* clone() const;

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

protected:
    Stage(const Stage& other);
};

#endif // STAGE_H
