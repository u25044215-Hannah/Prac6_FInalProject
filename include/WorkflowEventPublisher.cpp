#include "WorkflowEventPublisher.h"
#include "WorkflowEvent.h"
#include "WorkflowEventListener.h"
#include <algorithm>

void WorkflowEventPublisher::subscribe(EventType type, WorkflowEventListener* listener) {
    if (!listener) return;
    std::vector<WorkflowEventListener*>& group = listeners[type];
    if (std::find(group.begin(), group.end(), listener) == group.end())
        group.push_back(listener);
}

void WorkflowEventPublisher::unsubscribe(EventType type, WorkflowEventListener* listener) {
    auto it = listeners.find(type);
    if (it == listeners.end()) return;
    std::vector<WorkflowEventListener*>& group = it->second;
    group.erase(std::remove(group.begin(), group.end(), listener), group.end());
    if (group.empty()) listeners.erase(it);
}

void WorkflowEventPublisher::publish(const WorkflowEvent& event) {
    auto it = listeners.find(event.getType());
    if (it == listeners.end()) return;
    // Snapshot prevents iterator invalidation if a callback changes subscriptions.
    const std::vector<WorkflowEventListener*> snapshot = it->second;
    for (WorkflowEventListener* listener : snapshot) {
        // A listener removed during an earlier callback must not be called.
        auto current = listeners.find(event.getType());
        if (listener && current != listeners.end() &&
            std::find(current->second.begin(), current->second.end(), listener) != current->second.end())
            listener->onEvent(event);
    }
}
