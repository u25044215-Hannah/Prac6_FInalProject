#include "DefinitionBuilder.h"

#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

#include "Stage.h"
#include "SubProcess.h"
#include "WorkContainer.h"
#include "WorkNode.h"

// ---- Other people's classes. Every call into them is marked INTEGRATION. ----
#include "WorkflowDefinition.h"      // Person 1
#include "Task.h"                    // Person 3
#include "WorkItem.h"                // Person 3
#include "HumanTask.h"               // Person 3
#include "AutomatedTask.h"           // Person 3
#include "WorkItemDecorator.h"       // Person 3
#include "DecoratorFactory.h"        // Person 3 (isKnown / wrap by string key)
#include "EscalationHandler.h"       // Person 5
#include "EscalationLevel.h"         // Person 5
#include "TerminalEscalationLevel.h" // Person 5
#include "ApprovalHandler.h"         // Person 5
#include "ApprovalLevel.h"           // Person 5

namespace
{
    // Auto ids start with '@' and user ids may not, so they can never collide.
    const char AUTO_ID_MARK = '@';

    void checkContainers(const WorkContainer &c, std::vector<std::string> &errors)
    {
        if (c.getChildren().empty())
            errors.push_back("'" + c.getName() + "' has no children");
        for (std::size_t i = 0; i < c.getChildren().size(); ++i)
        {
            const WorkContainer *sub = dynamic_cast<const WorkContainer *>(c.getChildren()[i]);
            if (sub)
                checkContainers(*sub, errors);
        }
    }

    typedef std::map<std::string, std::vector<std::string> > Graph;

    // Depth-first search; colour 1 = on current path, 2 = finished.
    bool hasCycle(const Graph &g, const std::string &n, std::map<std::string, int> &colour)
    {
        colour[n] = 1;
        Graph::const_iterator it = g.find(n);
        if (it != g.end())
        {
            for (std::size_t i = 0; i < it->second.size(); ++i)
            {
                const std::string &m = it->second[i];
                int c = colour[m];
                if (c == 1)
                    return true;
                if (c == 0 && hasCycle(g, m, colour))
                    return true;
            }
        }
        colour[n] = 2;
        return false;
    }

} // namespace

DefinitionBuilder::DefinitionBuilder()
    : open(), definitionName(), root(nullptr),
      escalationHead(), escalationTail(nullptr),
      approvalHead(), approvalTail(nullptr),
      nextAutoId(1), usedIds(),
      dependencies(), assignments(), deadlines(), decorations() {}

DefinitionBuilder::~DefinitionBuilder() { reset(); }

void DefinitionBuilder::abort() { reset(); }

void DefinitionBuilder::reset()
{
    delete root; // deletes the whole tree (children owned by containers)
    root = nullptr;
    // Heads own their whole chain (handlers own `next`). If a definition
    // already took a shared copy, the chain lives on until it lets go.
    escalationHead.reset();
    approvalHead.reset();
    escalationTail = nullptr;
    approvalTail = nullptr;
    while (!open.empty())
        open.pop();
    definitionName.clear();
    usedIds.clear();
    dependencies.clear();
    assignments.clear();
    deadlines.clear();
    decorations.clear();
    nextAutoId = 1;
}

void DefinitionBuilder::requireProduct() const
{
    if (!root)
        throw std::logic_error("beginDefinition() must be called first");
}

WorkNode *DefinitionBuilder::requireNode(const std::string &id) const
{
    requireProduct();
    WorkNode *n = root->findById(id);
    if (!n)
        throw std::invalid_argument("unknown node id '" + id + "'");
    return n;
}

void DefinitionBuilder::checkIdFree(const std::string &id) const
{
    if (id.empty())
        throw std::invalid_argument("node id must not be empty");
    if (id[0] == AUTO_ID_MARK)
        throw std::invalid_argument("node id '" + id + "' must not start with '@' (reserved)");
    if (usedIds.count(id) != 0)
        throw std::invalid_argument("duplicate node id '" + id + "'");
}

std::string DefinitionBuilder::newAutoId(const char *prefix)
{
    for (;;)
    {
        std::ostringstream o;
        o << AUTO_ID_MARK << prefix << nextAutoId++;
        if (usedIds.insert(o.str()).second)
            return o.str();
    }
}

// ----------------------------- structure -----------------------------

void DefinitionBuilder::beginDefinition(const std::string &name)
{
    if (root)
        throw std::logic_error("a definition is already in progress; call build() or abort() first");
    if (name.empty())
        throw std::invalid_argument("definition name must not be empty");

    // The WorkflowDefinition is created in build(): its constructor needs the root.
    std::unique_ptr<Stage> top(new Stage("root", name));
    usedIds.insert("root");
    open.push(top.get());
    definitionName = name;
    root = top.release();
}

void DefinitionBuilder::beginStage(const std::string &name)
{
    requireProduct();
    std::unique_ptr<Stage> s(new Stage(newAutoId("stage_"), name));
    Stage *raw = s.get();
    open.top()->add(raw); // parent owns it from here on
    s.release();
    open.push(raw);
}

void DefinitionBuilder::beginSubProcess(const std::string &name)
{
    requireProduct();
    std::unique_ptr<SubProcess> s(new SubProcess(newAutoId("subprocess_"), name));
    SubProcess *raw = s.get();
    open.top()->add(raw);
    s.release();
    open.push(raw);
}

void DefinitionBuilder::endContainer()
{
    requireProduct();
    if (open.size() <= 1)
        throw std::logic_error("endContainer(): there is no open stage or sub-process to close");
    open.pop();
}

// ------------------------------- leaves -------------------------------

void DefinitionBuilder::addHumanTask(const std::string &id, const std::string &role)
{
    requireProduct();
    if (role.empty())
        throw std::invalid_argument("human task '" + id + "' needs a role");
    checkIdFree(id);
    // INTEGRATION: Person 3 ctor is HumanTask(id, name, Role* requiredRole).
    // TODO(Person 3): turn the `role` string into a Role* and pass it as the
    // third argument. It is only validated here for now, not stored.
    std::unique_ptr<HumanTask> t(new HumanTask(id, id));
    open.top()->add(t.get());
    t.release();
    usedIds.insert(id); // reserve only once the node is really in the tree
}

void DefinitionBuilder::addAutomatedTask(const std::string &id, const std::string &connector)
{
    requireProduct();
    if (connector.empty())
        throw std::invalid_argument("automated task '" + id + "' needs a connector");
    checkIdFree(id);
    // INTEGRATION: Person 3 ctor is AutomatedTask(id, name, Role*).
    // TODO(Person 3): AutomatedTask has no place to store the connector yet.
    std::unique_ptr<AutomatedTask> t(new AutomatedTask(id, id));
    open.top()->add(t.get());
    t.release();
    usedIds.insert(id);
}

// -------------------------------- rules --------------------------------

void DefinitionBuilder::dependsOn(const std::string &id, const std::string &prereqId)
{
    requireProduct();
    if (id == prereqId)
        throw std::invalid_argument("task '" + id + "' cannot depend on itself");
    dependencies.push_back(std::make_pair(id, prereqId)); // checked + applied in build()
}

void DefinitionBuilder::setRouting(const std::string &key)
{
    requireProduct();
    if (key.empty())
        throw std::invalid_argument("routing key must not be empty");
    Stage *stage = dynamic_cast<Stage *>(open.top());
    if (!stage)
        throw std::logic_error("setRouting(): the open container is not a stage");
    stage->setRoutingKey(key);
}

void DefinitionBuilder::setAssignment(const std::string &id, const std::string &key)
{
    requireProduct();
    if (key.empty())
        throw std::invalid_argument("assignment key must not be empty");
    assignments[id] = key;
}

void DefinitionBuilder::setDeadline(const std::string &id, int seconds)
{
    requireProduct();
    if (seconds <= 0)
        throw std::invalid_argument("deadline for '" + id + "' must be positive");
    deadlines[id] = seconds;
}

void DefinitionBuilder::decorateWith(const std::string &id, const std::string &key)
{
    requireProduct();
    if (key.empty())
        throw std::invalid_argument("decorator key must not be empty");
    decorations[id].push_back(key);
}

// ------------------------------- chains -------------------------------

void DefinitionBuilder::addEscalationLevel(const std::string &role, int seconds)
{
    requireProduct();
    if (role.empty())
        throw std::invalid_argument("escalation level needs a role");
    if (seconds <= 0)
        throw std::invalid_argument("escalation time must be positive");
    std::unique_ptr<EscalationHandler> level(new EscalationLevel(role, seconds)); // INTEGRATION: Person 5 ctor
    if (escalationTail)
    {
        escalationTail->setNext(level.get()); // INTEGRATION: Person 5 (if this throws, level is freed)
        escalationTail = level.release();
    }
    else
    {
        EscalationHandler *raw = level.release();
        escalationHead.reset(raw); // shared_ptr frees raw itself if this throws
        escalationTail = raw;
    }
}

void DefinitionBuilder::addApprovalLevel(const std::string &role, ApprovalLimits limits)
{
    requireProduct();
    if (role.empty())
        throw std::invalid_argument("approval level needs a role");
    std::unique_ptr<ApprovalHandler> level(new ApprovalLevel(role, limits)); // INTEGRATION: Person 5 ctor
    if (approvalTail)
    {
        approvalTail->setNext(level.get()); // INTEGRATION: Person 5
        approvalTail = level.release();
    }
    else
    {
        ApprovalHandler *raw = level.release();
        approvalHead.reset(raw);
        approvalTail = raw;
    }
}

// ------------------------------ validation ------------------------------

void DefinitionBuilder::validate() const
{
    requireProduct();
    std::vector<std::string> errors;

    if (open.size() != 1)
    {
        std::ostringstream m;
        m << (open.size() - 1) << " stage/sub-process still open: call endContainer()";
        errors.push_back(m.str());
    }

    checkContainers(*root, errors);

    // Dependencies: both ends must exist, the dependent must be a task, no cycles.
    Graph graph;
    for (std::size_t i = 0; i < dependencies.size(); ++i)
    {
        const std::string &id = dependencies[i].first;
        const std::string &pre = dependencies[i].second;
        if (!dynamic_cast<Task *>(root->findById(id)))
            errors.push_back("dependency: '" + id + "' is not a known task");
        if (!root->findById(pre))
            errors.push_back("dependency: '" + id + "' depends on unknown id '" + pre + "'");
        graph[id].push_back(pre);
    }
    std::map<std::string, int> colour;
    for (Graph::const_iterator it = graph.begin(); it != graph.end(); ++it)
    {
        if (colour[it->first] == 0 && hasCycle(graph, it->first, colour))
        {
            errors.push_back("dependency cycle involving '" + it->first + "'");
            break;
        }
    }

    for (std::map<std::string, std::string>::const_iterator it = assignments.begin();
         it != assignments.end(); ++it)
        if (!dynamic_cast<Task *>(root->findById(it->first)))
            errors.push_back("assignment: '" + it->first + "' is not a known task");

    for (std::map<std::string, int>::const_iterator it = deadlines.begin();
         it != deadlines.end(); ++it)
        if (!dynamic_cast<Task *>(root->findById(it->first)))
            errors.push_back("deadline: '" + it->first + "' is not a known task");

    for (std::map<std::string, std::vector<std::string> >::const_iterator it = decorations.begin();
         it != decorations.end(); ++it)
    {
        if (!dynamic_cast<WorkItem *>(root->findById(it->first)))
            errors.push_back("decorator: '" + it->first + "' is not a known work item");
        for (std::size_t k = 0; k < it->second.size(); ++k)
            if (!DecoratorFactory::isKnown(it->second[k])) // INTEGRATION: Person 3
                errors.push_back("decorator: unknown key '" + it->second[k] + "'");
    }

    if (!errors.empty())
    {
        std::string msg = "Invalid workflow definition:";
        for (std::size_t i = 0; i < errors.size(); ++i)
            msg += "\n  - " + errors[i];
        throw std::runtime_error(msg);
    }
}

// --------------------------------- build ---------------------------------

WorkflowDefinition *DefinitionBuilder::build()
{
    requireProduct();

    // Validation failure: nothing has been touched, so the builder stays usable
    // (fix the problem and call build() again, or call abort()).
    validate();

    // From here on the tree is modified. Any failure leaves it half-applied,
    // so reset everything and rethrow.
    try
    {
        // Rules that refer to nodes by id (all ids are known to exist now).
        for (std::size_t i = 0; i < dependencies.size(); ++i)
        {
            Task *t = dynamic_cast<Task *>(root->findById(dependencies[i].first));
            t->addDependency(dependencies[i].second); // INTEGRATION: Person 3
        }

        // TODO(Person 3): assignments and deadlines are collected and validated
        // but NOT applied yet: Task has no setter for an assignment policy or a
        // deadline. Apply them here once those exist:
        //   for each (id, key) in `assignments`  -> task->set...(key)
        //   for each (id, seconds) in `deadlines` -> task->set...(seconds)

        // Decorators last: wrap each item and swap the wrapper into the tree.
        // (Task-specific rules above are applied first because a wrapper is a
        // WorkItem, not a Task, and hides the Task API.)
        for (std::map<std::string, std::vector<std::string> >::const_iterator it = decorations.begin();
             it != decorations.end(); ++it)
        {
            WorkItem *item = dynamic_cast<WorkItem *>(root->findById(it->first));
            WorkContainer *parent = item->getParent();
            WorkItem *current = item;
            for (std::size_t k = 0; k < it->second.size(); ++k)
                current = DecoratorFactory::wrap(it->second[k], current); // INTEGRATION: Person 3
            if (current != item)
                // INTEGRATION: must NOT delete `item` (decorator owns it) and
                // must set `current`'s parent.
                parent->replaceChild(item, current);
        }

        // Terminal level closes the escalation chain.
        if (escalationHead)
        {
            std::unique_ptr<EscalationHandler> last(new TerminalEscalationLevel()); // INTEGRATION: Person 5
            escalationTail->setNext(last.get());
            escalationTail = last.release();
        }

        // Create the definition: it takes the root in its constructor. The
        // unique_ptr owns the tree from here, so the builder lets go of it
        // first (otherwise a throw would delete the tree twice).
        std::unique_ptr<WorkNode> rootNode(root);
        root = nullptr;
        while (!open.empty()) // these pointed into the tree
            open.pop();

        std::unique_ptr<WorkflowDefinition> def(
            new WorkflowDefinition(definitionName, std::move(rootNode))); // INTEGRATION: Person 1
        if (escalationHead)
            def->setEscalationChain(escalationHead); // INTEGRATION: Person 1 (shares the head)
        if (approvalHead)
            def->setApprovalChain(approvalHead); // INTEGRATION: Person 1

        reset(); // drops the builder's own shared copies; the definition keeps its own
        return def.release();
    }
    catch (...)
    {
        reset();
        throw;
    }
}