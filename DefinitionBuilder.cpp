#include "DefinitionBuilder.h"

#include <memory>
#include <sstream>
#include <stdexcept>

#include "Stage.h"
#include "SubProcess.h"
#include "WorkContainer.h"

// ---- Other people's classes. Every call into them is marked INTEGRATION. ----
#include "WorkflowDefinition.h"      // Person 1
#include "Task.h"                    // Person 3
#include "WorkItem.h"                // Person 3
#include "HumanTask.h"               // Person 3
#include "AutomatedTask.h"           // Person 3
#include "DecoratorFactory.h"        // Person 3
#include "EscalationHandler.h"       // Person 5
#include "EscalationLevel.h"         // Person 5
#include "TerminalEscalationLevel.h" // Person 5
#include "ApprovalHandler.h"         // Person 5
#include "ApprovalLevel.h"           // Person 5

namespace
{

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

    typedef std::map<std::string, std::vector<std::string>> Graph;

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
    : open(), product(nullptr), root(nullptr),
      escalationLevels(), approvalLevels(), nextAutoId(1), usedIds(),
      dependencies(), assignments(), deadlines(), decorations() {}

DefinitionBuilder::~DefinitionBuilder() { reset(); }

void DefinitionBuilder::reset()
{
    delete root;
    root = nullptr; // deletes the whole tree
    delete product;
    product = nullptr;
    // Assumption: handlers do not delete their `next` (we free each level once).
    for (std::size_t i = 0; i < escalationLevels.size(); ++i)
        delete escalationLevels[i];
    for (std::size_t i = 0; i < approvalLevels.size(); ++i)
        delete approvalLevels[i];
    escalationLevels.clear();
    approvalLevels.clear();
    while (!open.empty())
        open.pop();
    usedIds.clear();
    dependencies.clear();
    assignments.clear();
    deadlines.clear();
    decorations.clear();
    nextAutoId = 1;
}

void DefinitionBuilder::requireProduct() const
{
    if (!product || !root)
        throw std::logic_error("beginDefinition() must be called first");
}

void DefinitionBuilder::registerId(const std::string &id)
{
    if (id.empty())
        throw std::invalid_argument("node id must not be empty");
    if (!usedIds.insert(id).second)
        throw std::invalid_argument("duplicate node id '" + id + "'");
}

std::string DefinitionBuilder::newAutoId(const char *prefix)
{
    for (;;)
    {
        std::ostringstream o;
        o << prefix << nextAutoId++;
        if (usedIds.insert(o.str()).second)
            return o.str();
    }
}

// ----------------------------- structure -----------------------------

void DefinitionBuilder::beginDefinition(const std::string &name)
{
    if (product || root)
        throw std::logic_error("a definition is already in progress; call build() first");
    if (name.empty())
        throw std::invalid_argument("definition name must not be empty");

    std::unique_ptr<WorkflowDefinition> def(new WorkflowDefinition(name)); // INTEGRATION: Person 1
    std::unique_ptr<Stage> top(new Stage("root", name));
    usedIds.insert("root");
    open.push(top.get());
    product = def.release();
    root = top.release();
}

void DefinitionBuilder::beginStage(const std::string &name)
{
    requireProduct();
    std::unique_ptr<Stage> s(new Stage(newAutoId("stage_"), name));
    open.top()->add(s.get());
    open.push(s.release());
}

void DefinitionBuilder::beginSubProcess(const std::string &name)
{
    requireProduct();
    std::unique_ptr<SubProcess> s(new SubProcess(newAutoId("subprocess_"), name));
    open.top()->add(s.get());
    open.push(s.release());
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
    registerId(id);
    std::unique_ptr<HumanTask> t(new HumanTask(id, role)); // INTEGRATION: Person 3 ctor
    open.top()->add(t.get());
    t.release();
}

void DefinitionBuilder::addAutomatedTask(const std::string &id, const std::string &connector)
{
    requireProduct();
    if (connector.empty())
        throw std::invalid_argument("automated task '" + id + "' needs a connector");
    registerId(id);
    std::unique_ptr<AutomatedTask> t(new AutomatedTask(id, connector)); // INTEGRATION: Person 3 ctor
    open.top()->add(t.get());
    t.release();
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
    open.top()->setRoutingKey(key);
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
    escalationLevels.push_back(level.get());
    level.release();
    if (escalationLevels.size() > 1)
        escalationLevels[escalationLevels.size() - 2]->setNext(escalationLevels.back());
}

void DefinitionBuilder::addApprovalLevel(const std::string &role, ApprovalLimits limits)
{
    requireProduct();
    if (role.empty())
        throw std::invalid_argument("approval level needs a role");
    std::unique_ptr<ApprovalHandler> level(new ApprovalLevel(role, limits)); // INTEGRATION: Person 5 ctor
    approvalLevels.push_back(level.get());
    level.release();
    if (approvalLevels.size() > 1)
        approvalLevels[approvalLevels.size() - 2]->setNext(approvalLevels.back());
}

// ------------------------------ validation ------------------------------

void DefinitionBuilder::validate() const
{
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

    for (std::map<std::string, std::vector<std::string>>::const_iterator it = decorations.begin();
         it != decorations.end(); ++it)
    {
        if (!dynamic_cast<WorkItem *>(root->findById(it->first)))
            errors.push_back("decorator: '" + it->first + "' is not a known task");
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
    validate(); // throws with every problem; nothing has been applied yet

    // Rules that refer to nodes by id (all ids are known to exist now).
    for (std::size_t i = 0; i < dependencies.size(); ++i)
    {
        Task *t = dynamic_cast<Task *>(root->findById(dependencies[i].first));
        t->getDependencies().add(dependencies[i].second); // INTEGRATION: non-const getDependencies()
    }
    for (std::map<std::string, std::string>::const_iterator it = assignments.begin();
         it != assignments.end(); ++it)
    {
        Task *t = dynamic_cast<Task *>(root->findById(it->first));
        t->setAssignmentKey(it->second); // INTEGRATION: Person 3
    }
    for (std::map<std::string, int>::const_iterator it = deadlines.begin();
         it != deadlines.end(); ++it)
    {
        Task *t = dynamic_cast<Task *>(root->findById(it->first));
        t->setDeadlineSeconds(it->second); // INTEGRATION: Person 3
    }

    // Decorators last: wrap each task and swap the wrapper into the tree.
    for (std::map<std::string, std::vector<std::string>>::const_iterator it = decorations.begin();
         it != decorations.end(); ++it)
    {
        WorkItem *item = dynamic_cast<WorkItem *>(root->findById(it->first));
        WorkContainer *parent = item->getParent();
        WorkItem *current = item;
        for (std::size_t k = 0; k < it->second.size(); ++k)
            current = DecoratorFactory::wrap(it->second[k], current); // INTEGRATION: Person 3
        if (current != item)
            parent->replaceChild(item, current);
    }

    // Terminal level closes the escalation chain.
    if (!escalationLevels.empty())
    {
        std::unique_ptr<EscalationHandler> last(new TerminalEscalationLevel()); // INTEGRATION: Person 5
        escalationLevels.push_back(last.get());
        last.release();
        escalationLevels[escalationLevels.size() - 2]->setNext(escalationLevels.back());
        product->setEscalationChain(escalationLevels.front()); // INTEGRATION: Person 1
    }
    if (!approvalLevels.empty())
        product->setApprovalChain(approvalLevels.front()); // INTEGRATION: Person 1

    product->setRoot(root); // INTEGRATION: Person 1 (takes ownership)

    // Ownership has moved to the definition: forget it before reset().
    WorkflowDefinition *result = product;
    product = nullptr;
    root = nullptr;
    escalationLevels.clear();
    approvalLevels.clear();
    reset();
    return result;
}