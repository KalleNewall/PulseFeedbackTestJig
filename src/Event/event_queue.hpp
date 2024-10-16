
#pragma once
#include <queue>
#include "event.hpp"

class EventQueue {
public:
    void push(const Event& event);
    Event pop();
    bool isEmpty() const;

private:
    std::queue<Event> queue;
};
    