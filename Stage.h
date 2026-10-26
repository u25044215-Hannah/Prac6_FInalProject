#ifndef STAGE_H
#define STAGE_H

#include "WorkContainer.h"

class Stage : public WorkContainer {
public:
    Stage(const std::string& id, const std::string& name);
    virtual ~Stage();

    virtual void start();
    virtual void cancel();
    virtual bool isComplete() const;

    /// Deep clone (covariant return).
    virtual Stage* clone() const;

    /// Calls v.visit(*this), then accept(v) on every child.
    virtual void accept(WorkNodeVisitor& v);

    virtual WorkNodeIterator* createIterator();        ///< depth-first
    virtual WorkNodeIterator* createReadyIterator();   ///< only ready work items

protected:
    Stage(const Stage& other);
};

#endif // STAGE_H