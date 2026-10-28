#include "Stage.h"

#include "WorkNodeVisitor.h"    // Person 7
#include "DepthFirstIterator.h" // Person 7
#include "ReadyItemIterator.h"  // Person 7

Stage::Stage(const std::string &id, const std::string &name)
    : WorkContainer(id, name) {}

Stage::Stage(const Stage &other) : WorkContainer(other) {}

Stage::~Stage() {}

void Stage::start() { startChildren(); }

void Stage::cancel()
{
    // Finished children cannot be cancelled (their state is final).
    for (std::size_t i = 0; i < children.size(); ++i)
        if (!children[i]->isComplete())
            children[i]->cancel();
}

bool Stage::isComplete() const { return WorkContainer::isComplete(); }

Stage *Stage::clone() const { return new Stage(*this); }

void Stage::accept(WorkNodeVisitor &v)
{
    v.visit(*this);
    for (std::size_t i = 0; i < children.size(); ++i)
        children[i]->accept(v);
}

WorkNodeIterator *Stage::createIterator()
{
    return new DepthFirstIterator(this); // INTEGRATION: Person 7 ctor(WorkNode*)
}

WorkNodeIterator *Stage::createReadyIterator()
{
    return new ReadyItemIterator(this); // INTEGRATION: Person 7 ctor(WorkNode*)
}