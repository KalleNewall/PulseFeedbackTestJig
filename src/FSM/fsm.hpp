
#pragma once
#include "fsm_state.hpp"
#include "../Event/event_queue.hpp"

class FSM {
public:
    FSM(FSMState* initialState, EventQueue* eventQueue);
    void transitionTo(FSMState* state);
    void update(); // Call this in the main loop

private:
    FSMState* currentState;
    EventQueue* eventQueue;
};
    