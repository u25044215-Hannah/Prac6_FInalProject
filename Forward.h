#ifndef TIVIDY_STUBS_FORWARD_H
#define TIVIDY_STUBS_FORWARD_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <stdexcept>


class Condition {
public:
    std::string field, op, value;
    bool evaluate(const class WorkflowData& data) const; 
};


class WorkflowData {
    std::map<std::string, std::string> values_;
public:
    std::string get(const std::string& key) const {
        auto it = values_.find(key);
        return it == values_.end() ? "" : it->second;
    }
    void set(const std::string& key, const std::string& value) { values_[key] = value; }
    bool has(const std::string& key) const { return values_.count(key) > 0; }
};

class Role {
public:
    std::string name;
    explicit Role(std::string n = "") : name(std::move(n)) {}
};

class Participant {
public:
    std::string id;
    std::vector<Role> roles;
    explicit Participant(std::string pid = "") : id(std::move(pid)) {}
    bool hasRole(const Role& r) const {
        for (const auto& role : roles) if (role.name == r.name) return true;
        return false;
    }
};

class DependencySet {
    std::vector<std::string> prerequisiteIds_;
public:
    void add(const std::string& nodeId) { prerequisiteIds_.push_back(nodeId); }
    
    bool isSatisfied() const { return true; }
};


class Task; 
class AssignmentStrategy {
public:
    virtual ~AssignmentStrategy() = default;
    virtual Participant* selectParticipant(const Task& item, const std::vector<Participant*>& candidates) = 0;
};


enum class EventType {
    WORK_ITEM_STATE_CHANGED, DEADLINE_EXPIRED, THRESHOLD_BREACHED,
    EXTERNAL_EVENT, INTEGRATION_FAILED, ACCESS_DENIED
};

struct WorkflowEvent {
    EventType type;
    std::string sourceId;
    std::string instanceId;
    WorkflowData payload;
};

class WorkflowEventPublisher {
public:
    void publish(const WorkflowEvent& ) {}
};


class WorkflowCommand; 
class ActionHistory {
public:
    void appendNote(const std::string& ) {}
};


class WorkContainer; 
class WorkNodeVisitor; 
class WorkNodeIterator; 

class WorkNode {
protected:
    std::string id_;
    std::string name_;
    WorkContainer* parent_ = nullptr;
public:
    WorkNode(std::string id, std::string name) : id_(std::move(id)), name_(std::move(name)) {}
    virtual ~WorkNode() = default;

    virtual void start() = 0;
    virtual void cancel() = 0;
    virtual bool isComplete() const = 0;
    virtual WorkNode* clone() const = 0;

    std::string getId() const { return id_; }
    std::string getName() const { return name_; }
};


class IllegalTransitionException : public std::logic_error {
public:
    IllegalTransitionException(const std::string& fromState, const std::string& operation)
        : std::logic_error("Illegal transition: '" + operation + "' is not valid from state '" + fromState + "'") {}
};

#endif
