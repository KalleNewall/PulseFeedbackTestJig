
#include "event_queue.hpp"

void EventQueue::push(const Event& event) {
    queue.push(event);
}

Event EventQueue::pop() {
    if (!queue.empty()) {
        Event event = queue.front();
        queue.pop();
        return event;
    }
    return { EventType::None, 0 }; 
}

bool EventQueue::isEmpty() const {
    return queue.empty();
}
    