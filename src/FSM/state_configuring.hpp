
#pragma once
#include "fsm_state.hpp"

class StateConfiguring : public FSMState {
public:
    void handle() override;
    void handleEvent(const Event& event) override;
    void onEnter() override; 
    void onExit() override;  
};
    