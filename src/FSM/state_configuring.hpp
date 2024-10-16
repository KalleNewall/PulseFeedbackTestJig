
#pragma once
#include "fsm_state.hpp"

class StateConfiguring : public FSMState {
public:
    void handle() override;
    void handleEvent(const Event& event) override;
    void onEnter() override; // Optional: override to add entry logic
    void onExit() override;  // Optional: override to add exit logic
};
    