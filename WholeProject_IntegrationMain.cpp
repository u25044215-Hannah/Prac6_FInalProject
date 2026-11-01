// Person 7 demonstration: two industry scenarios, tree traversal, reporting,
// filtering, scenario status, workflow cloning, and exception paths.
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "Stage.h"
#include "SubProcess.h"
#include "HumanTask.h"
#include "WorkflowDefinition.h"
#include "WorkflowInstance.h"
#include "WorkNodeIterator.h"
#include "WorkItem.h"
#include "Participant.h"
#include "DepthFirstIterator.h"
#include "LevelOrderIterator.h"
#include "ReadyItemIterator.h"
#include "ProgressVisitor.h"
#include "BottleneckVisitor.h"
#include "CycleTimeVisitor.h"
#include "DefinitionValidationVisitor.h"
#include "CommandProcessor.h"
#include "AssignCommand.h"
#include "StartCommand.h"
#include "CompleteCommand.h"
#include "RecoveryManager.h"
#include "RoleAssignment.h"
#include "SkillAssignment.h"
#include "Role.h"
#include "WorkItemState.h"

struct Scenario {
    std::string industry;
    std::unique_ptr<WorkflowDefinition> definition;
    std::unique_ptr<WorkflowInstance> instance;
    std::map<std::string, double> illustrativeSeconds;
};

static int passed = 0, failed = 0;
static void check(bool ok, const std::string& description) {
    std::cout << (ok ? "[PASS] " : "[FAIL] ") << description << '\n';
    (ok ? passed : failed)++;
}

static std::string nodeType(WorkNode* n) {
    if (dynamic_cast<Stage*>(n)) return "Stage";
    if (dynamic_cast<SubProcess*>(n)) return "SubProcess";
    if (dynamic_cast<WorkItem*>(n)) return "Task";
    return "Node";
}

static std::vector<std::string> traverse(WorkNodeIterator& it, bool display) {
    std::vector<std::string> ids;
    while (it.hasNext()) {
        WorkNode* n = it.next();
        if (!n) throw std::runtime_error("Iterator returned a null node");
        ids.push_back(n->getId());
        if (display) {
            std::cout << "  " << std::left << std::setw(14) << n->getId()
                      << std::setw(14) << nodeType(n) << n->getName();
            if (auto* w = dynamic_cast<WorkItem*>(n))
                std::cout << " [" << w->stateName() << "]";
            std::cout << '\n';
        }
    }
    return ids;
}

static Scenario makeScenario(bool healthcare) {
    Scenario s;
    s.industry = healthcare ? "Healthcare - patient admission" : "Logistics - order fulfilment";
    std::unique_ptr<Stage> root(new Stage(healthcare ? "H-ROOT" : "L-ROOT", s.industry));
    SubProcess* first = new SubProcess(healthcare ? "H-INTAKE" : "L-RECEIVE", healthcare ? "Intake" : "Order receiving");
    SubProcess* second = new SubProcess(healthcare ? "H-CARE" : "L-SHIP", healthcare ? "Care and discharge" : "Picking and dispatch");
    root->add(first);
    root->add(second);
    if (healthcare) {
        first->add(new HumanTask("H-REGISTER", "Register patient"));
        first->add(new HumanTask("H-TRIAGE", "Assess patient"));
        second->add(new HumanTask("H-TREAT", "Provide treatment"));
        second->add(new HumanTask("H-DISCHARGE", "Prepare discharge"));
        s.illustrativeSeconds = {{"H-REGISTER",120},{"H-TRIAGE",480},{"H-TREAT",1800},{"H-DISCHARGE",300}};
    } else {
        first->add(new HumanTask("L-ORDER", "Capture order"));
        first->add(new HumanTask("L-STOCK", "Verify stock"));
        second->add(new HumanTask("L-PICK", "Pick and pack"));
        second->add(new HumanTask("L-DISPATCH", "Dispatch delivery"));
        s.illustrativeSeconds = {{"L-ORDER",60},{"L-STOCK",180},{"L-PICK",600},{"L-DISPATCH",240}};
    }
    s.definition.reset(new WorkflowDefinition(s.industry, std::unique_ptr<WorkNode>(root.release())));
    s.definition->setVersion(1);
    s.definition->publish();
    s.instance.reset(new WorkflowInstance(healthcare ? "H-RUN-01" : "L-RUN-01", *s.definition, WorkflowData()));
    return s;
}

static void report(Scenario& s) {
    WorkNode& root = s.instance->getRoot();
    std::cout << "\n=== " << s.industry << " : REPORTS ===\n";
    ProgressVisitor progress;
    root.accept(progress);
    std::cout << std::fixed << std::setprecision(1)
              << "Progress (completed tasks): " << progress.percentComplete() << "%\n";
    auto duration = [&s](const WorkItem& item, double& secs) {
        auto it = s.illustrativeSeconds.find(item.getId());
        if (it == s.illustrativeSeconds.end()) return false;
        secs = it->second;
        return true;
    };
    CycleTimeVisitor cycle(duration);
    BottleneckVisitor bottleneck(duration);
    root.accept(cycle);
    root.accept(bottleneck);
    std::cout << "Average SAMPLE task duration: " << cycle.averageSeconds() << " seconds\n";
    std::cout << "Longest SAMPLE durations: ";
    for (const auto& id : bottleneck.slowestItems(2)) std::cout << id << " ";
    std::cout << "\n(Note: durations are scenario test fixtures, not measured runtime.)\n";
    DefinitionValidationVisitor validation;
    root.accept(validation);
    const auto errors = validation.errors();
    std::cout << "Definition validation: " << (errors.empty() ? "PASS" : "FAIL") << '\n';
    for (const auto& e : errors) std::cout << "  - " << e << '\n';
    std::cout << "Instance history entries: " << s.instance->getHistory().log().size() << '\n';
    for (const auto& note : s.instance->getHistory().log()) std::cout << "  " << note << '\n';
}

static void listNodes(Scenario& s, int order) {
    WorkNode& root = s.instance->getRoot();
    std::cout << "\n=== " << s.industry << " ===\n";
    if (order == 1) { std::unique_ptr<WorkNodeIterator> it(root.createIterator()); traverse(*it,true); }
    else if (order == 2) { LevelOrderIterator it(&root); traverse(it,true); }
    else { std::unique_ptr<WorkNodeIterator> it(root.createReadyIterator()); traverse(*it,true); }
}

static std::vector<WorkItem*> tasks(Scenario& s) {
    std::vector<WorkItem*> result;
    std::unique_ptr<WorkNodeIterator> it(s.instance->getRoot().createIterator());
    while(it->hasNext()) if (auto* w=dynamic_cast<WorkItem*>(it->next())) result.push_back(w);
    return result;
}

static void makeAvailable(Scenario& s) {
    for (auto* w : tasks(s)) if (w->stateName()=="Created") w->makeAvailable();
}

static void testScenario(Scenario& s) {
    std::cout << "\n========== TEST: " << s.industry << " ==========\n";
    WorkNode& root = s.instance->getRoot();
    std::unique_ptr<WorkNodeIterator> dfs(root.createIterator());
    auto d = traverse(*dfs, false);
    check(d.size() == 7, "DFS visits 1 root + 2 subprocesses + 4 tasks");
    LevelOrderIterator bfs(&root);
    auto b = traverse(bfs, false);
    check(b.size()==7 && b.front()==root.getId(), "BFS traverses all nodes starting at root");
    check(d != b, "DFS and BFS give different valid traversal orders");
    // Iterator exhaustion must report an error rather than return a stale node.
    try {
        DepthFirstIterator empty(nullptr);
        (void)empty.next();
        check(false, "Exhausted iterator throws");
    } catch (const std::out_of_range&) {
        check(true, "Exhausted iterator throws");
    }
    // Negative definition test, independent of the actual industry tree.
    {
        Stage duplicate("DUP-ROOT", "Intentionally invalid test tree");
        duplicate.add(new HumanTask("DUP-ID", "First"));
        duplicate.add(new HumanTask("DUP-ID", "Second"));
        DefinitionValidationVisitor invalid;
        duplicate.accept(invalid);
        check(!invalid.errors().empty(), "Definition validation rejects duplicate IDs");
    }
    DefinitionValidationVisitor validation;
    root.accept(validation);
    check(validation.errors().empty(), "Visitor validates unique, non-empty node IDs");
    ProgressVisitor p0;
    root.accept(p0);
    check(std::abs(p0.percentComplete())<0.001, "Initial progress is 0%");
    {
        std::unique_ptr<WorkNodeIterator> ready(root.createReadyIterator());
        check(traverse(*ready,false).empty(), "No tasks ready before availability");
    }
    makeAvailable(s);
    {
        std::unique_ptr<WorkNodeIterator> ready(root.createReadyIterator());
        check(traverse(*ready,false).size()==4, "Ready filter returns all 4 available tasks");
    }
    Participant actor("operator-1");
    auto ts = tasks(s);
    // Explicit state actions are used rather than starting the whole container:
    // the latter orchestrates transitions automatically and depends on routing.
    ts[0]->assign(actor,actor);
    ts[0]->acceptAssignment(actor);
    ts[0]->start();
    ts[0]->complete(WorkflowData(),actor);
    check(ts[0]->isComplete(), "Task transitions Available -> Assigned -> InProgress -> Completed");
    ProgressVisitor p1;
    root.accept(p1);
    check(std::abs(p1.percentComplete()-25.0)<0.001, "Visitor reports 25% completion");
    {
        std::unique_ptr<WorkNodeIterator> ready(root.createReadyIterator());
        check(traverse(*ready,false).size()==3, "Ready filter excludes completed task");
    }
    try { ts[0]->hold(); check(false,"Completed task rejects illegal hold"); }
    catch (const std::logic_error&) { check(true,"Completed task rejects illegal hold"); }
    // Cloning tests prototype isolation: the copied task starts in Created state.
    std::unique_ptr<WorkNode> cloned(root.clone());
    check(cloned.get()!=&root && cloned->findById(ts[0]->getId())!=ts[0], "Prototype makes independent nodes");
    auto* clonedTask=dynamic_cast<WorkItem*>(cloned->findById(ts[0]->getId()));
    check(clonedTask && clonedTask->stateName()=="Created", "Prototype resets task execution state");
    report(s);
}


// Cross-module integration: role/skill selection, Command, History and Memento.
static void testCoreIntegration(Scenario& s) {
    std::cout << "\n========== INTEGRATION: " << s.industry << " ==========\n";
    // Run on a fresh workflow instance, independently of the visitor tests.
    s.instance.reset(new WorkflowInstance(s.industry.substr(0,1)+"-INTEGRATION", *s.definition, WorkflowData()));
    auto items=tasks(s);
    auto* task=dynamic_cast<Task*>(items.at(0));
    check(task!=nullptr,"Concrete task found through composite tree");
    if (!task) return;
    Participant operator1("qualified-operator"), operator2("unqualified-operator");
    Role required("operator");
    operator1.addRole(required);
    operator1.addSkill("safe-handling");
    task->addRequiredSkill("safe-handling");
    // These are the real RoleAssignment/SkillAssignment strategies.
    std::vector<Participant*> people{&operator2,&operator1};
    RoleAssignment roleStrategy;
    SkillAssignment skillStrategy;
    // Setting the required role is supported only in the constructor, so
    // test role selection against an independent task template.
    HumanTask roleTask("ROLE-CHECK", "Role eligible task", &required);
    check(roleStrategy.selectParticipant(roleTask,people)==&operator1,
          "Strategy: role-based assignment chooses qualified participant");
    check(skillStrategy.selectParticipant(*task,people)==&operator1,
          "Strategy: skills-based assignment chooses qualified participant");
    CommandProcessor commands;
    RecoveryManager recovery;
    task->makeAvailable();
    recovery.checkpoint(*s.instance,"before-assignment");
    check(recovery.hasCheckpoint(*s.instance,"before-assignment"),"Memento: saved checkpoint");
    commands.submit(new AssignCommand(*task,operator1,&operator1),*s.instance);
    check(task->stateName()=="Assigned","Command: assign changes state");
    check(!s.instance->getHistory().entries().empty(),"History: command appended");
    // Checkpoint/rollback restores the work item's previous state.
    recovery.rollback(*s.instance,"before-assignment");
    check(task->stateName()=="Available","Recovery: rollback restores Available");
    check(!s.instance->getHistory().log().empty(),"History: rollback recorded");
    recovery.discard(*s.instance,"before-assignment");
    check(!recovery.hasCheckpoint(*s.instance,"before-assignment"),"Recovery: discarded checkpoint");
    commands.submit(new AssignCommand(*task,operator1,&operator1),*s.instance);
    commands.submit(new StartCommand(*task,&operator1),*s.instance);
    check(task->stateName()=="InProgress","Command: start changes state");
    WorkflowData result;
    result.set("outcome","ok");
    commands.submit(new CompleteCommand(*task,result,&operator1),*s.instance);
    check(task->isComplete(),"Command: completion reaches terminal state");
    check(s.instance->getHistory().entries().size()==4,"Command history retains successful actions");
    try { commands.submit(new StartCommand(*task,&operator1),*s.instance);
          check(false,"Illegal transition rejected through command processor"); }
    catch(const std::exception&) { check(true,"Illegal transition rejected through command processor"); }
    check(!s.instance->getHistory().log().empty(),"History: failure recorded");
    ProgressVisitor progress;
    s.instance->getRoot().accept(progress);
    check(std::abs(progress.percentComplete()-25.0)<0.001,
          "Visitor: progress reflects command-driven completion");
}

static void scenarioMenu(Scenario& s) {
    while (true) {
        std::cout << "\n" << s.industry << "\n"
                  << "1. Show depth-first traversal\n2. Show level-order traversal\n"
                  << "3. Show ready tasks\n4. Show reports and history\n"
                  << "5. Run automated scenario checks\n6. Run command/strategy/recovery integration\n0. Back\nChoice: ";
        int option;
        if (!(std::cin >> option)) { if (std::cin.eof()) return; std::cin.clear(); std::cin.ignore(10000,'\n'); std::cout<<"Invalid input\n"; continue; }
        if(option==0) return;
        try {
            
if (option == 3) {
    // Move newly created tasks into the Available state.
    makeAvailable(s);

    // Use the real ReadyItemIterator to display available tasks.
    listNodes(s, option);
}
else if (option == 1 || option == 2) {
    listNodes(s, option);
}

            else if (option==4) report(s);
            else if (option==5) {
                // Reset test state using a fresh independently cloned instance.
                s.instance.reset(new WorkflowInstance(s.industry.substr(0,1)+"-RUN-RESET", *s.definition, WorkflowData()));
                testScenario(s);
            } else if(option==6) testCoreIntegration(s); else std::cout << "Unknown option\n";
        } catch(const std::exception& e) { std::cout << "[ERROR] " << e.what() << '\n'; }
    }
}

int main(int argc, char** argv) {
    try {
        Scenario health=makeScenario(true), logistics=makeScenario(false);
        if (argc>1 && std::string(argv[1])=="--test") {
            testScenario(health);
            testScenario(logistics);
            testCoreIntegration(health);
            testCoreIntegration(logistics);
            std::cout << "\nTOTAL: " << passed << " passed, " << failed << " failed\n";
            return failed?1:0;
        }
        while(true) {
            std::cout << "\n========== TIVIDY / PERSON 7 DEMO ==========\n"
                      << "1. Healthcare workflow\n2. Logistics workflow\n"
                      << "3. Test both industries\n0. Exit\nChoice: ";
            int option;
            if(!(std::cin >> option)) { if(std::cin.eof()) break; std::cin.clear(); std::cin.ignore(10000,'\n'); std::cout<<"Invalid input\n"; continue; }
            if(option==0) break;
            if(option==1) scenarioMenu(health);
            else if(option==2) scenarioMenu(logistics);
            else if(option==3) {
                health=makeScenario(true); logistics=makeScenario(false);
                testScenario(health); testScenario(logistics);
                testCoreIntegration(health); testCoreIntegration(logistics);
                std::cout << "\nTOTAL: " << passed << " passed, " << failed << " failed\n";
            } else std::cout << "Unknown option\n";
        }
        return failed?1:0;
    } catch(const std::exception& e) {
        std::cerr << "Fatal demo error: " << e.what() << '\n'; return 2;
    }
}
