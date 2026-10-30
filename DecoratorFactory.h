#ifndef DECORATORFACTORY_H
#define DECORATORFACTORY_H

#include <functional>
#include <string>

class WorkItem;

class DecoratorFactory {
public:
    /// Creator: receives the item to wrap, returns the new decorator.
    /// It takes ownership of `inner` by passing it to the decorator constructor.
    typedef std::function<WorkItem*(WorkItem* inner)> Creator;

    /// Registers (or replaces) the creator for `key`.
    /// Throws std::invalid_argument for an empty key or an empty creator.
    static void registerDecorator(const std::string& key, Creator creator);

    /// True if a creator is registered for `key`.
    static bool isKnown(const std::string& key);

    /// Wraps `inner` using the creator for `key`. Takes ownership of `inner`
    /// on success. If it throws (unknown key, or the creator throws), the
    /// caller still owns `inner`.
    static WorkItem* wrap(const std::string& key, WorkItem* inner);

    /// Removes all registrations (useful in tests).
    static void clear();

private:
    DecoratorFactory() = delete;
};

#endif