
#pragma once
#include "../Event/event.hpp"

class FSM;

class FSMState {
public:
    virtual void handle() = 0; // Periodic logic for the state
    virtual void handleEvent(const Event& event) = 0; // Event handling logic
    virtual void onEnter() {} // Called when entering the state
    virtual void onExit() {}  // Called when exiting the state
    virtual ~FSMState() = default;
};
    