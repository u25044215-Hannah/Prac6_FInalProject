#ifndef STAGE_H
#define STAGE_H

#include "WorkContainer.h"

class Stage : public WorkContainer
{
public:
    Stage(const std::string &id, const std::string &name);
    virtual ~Stage();

    virtual void start();
    virtual void cancel();
    virtual bool isComplete() const;
    virtual Stage *clone() const;

protected:
    Stage(const Stage &other);
};

#endif