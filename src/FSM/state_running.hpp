
#pragma once
#include "fsm_state.hpp"

const int servoPin = 18;

class StateRunning : public FSMState {
public:
    void handle() override;
    void handleEvent(const Event& event) override;
    void onEnter() override; 
    void onExit() override;  
};
    