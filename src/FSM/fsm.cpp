
#include "fsm.hpp"

FSM::FSM(FSMState* initialState, EventQueue* eventQueue)
    : currentState(initialState), eventQueue(eventQueue) {
    if (currentState) {
        currentState->onEnter(); // Call onEnter for the initial state
    }
}

void FSM::transitionTo(FSMState* state) {
    if (currentState) {
        currentState->onExit(); // Call onExit for the current state
    }
    currentState = state;
    if (currentState) {
        currentState->onEnter(); // Call onEnter for the new state
    }
}

void FSM::update() {
    if (currentState) {
        if (!eventQueue->isEmpty()) {
            Event event = eventQueue->pop();
            currentState->handleEvent(event); // Dispatch event to the current state
        }
        currentState->handle(); // State-specific periodic logic
    }
}
    