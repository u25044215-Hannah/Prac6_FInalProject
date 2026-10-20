/**
 * @file WorkNode.h
 * @brief PLACEHOLDER for Ty's Composite root. Replace with the real class.
 */
#ifndef TIVIDY_WORK_NODE_H
#define TIVIDY_WORK_NODE_H

#include <map>
#include <string>

class WorkNode {
public:
    virtual ~WorkNode() {}
    virtual std::string getId() const = 0;
    virtual void start() = 0;
    virtual void cancel() = 0;
    virtual bool isComplete() const = 0;
    ///Prototype: deep copy of this node and all descendants (caller owns result)
    virtual WorkNode* clone() const = 0;
    /// emento hook: write "node id -> state name" for this node and descendants
    virtual void captureState(std::map<std::string, std::string>& out) const { (void)out; }
    ///Memento hook: restore states previously written by captureState()
    virtual void restoreState(const std::map<std::string, std::string>& in) { (void)in; }
};

#endif