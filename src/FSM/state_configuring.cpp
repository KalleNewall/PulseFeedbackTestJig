
#include "state_configuring.hpp"
#include <Arduino.h>
#include "Menu/menu.hpp"

#include "state_running.hpp"
#include "fsm.hpp"


Menu* currentMenu = &Main;

//FSM and runningState instantiated in main.cpp
extern FSM* fsm;
extern StateRunning runningState;


void StateConfiguring::handle() {
    ;
}

void StateConfiguring::handleEvent(const Event& event) {
    switch (event.type) {
        case EventType::JoyRight:
            Serial.println("Pressed Right in Config State");
            
            break;
        case EventType::JoyLeft:
            Serial.println("Pressed Left in Config State");
            currentMenu = &Main;
            renderMenu(currentMenu);
            delay(200); 
            
            break;
        case EventType::JoyDown:
            Serial.println("Pressed Down in Config State");
            currentMenu->selected = min(currentMenu->size - 1, currentMenu->selected + 1);
            renderMenu(currentMenu);
            delay(200);

            break;
        case EventType::JoyUp:
            Serial.println("Pressed Up in Config State");
            currentMenu->selected = max(0, currentMenu->selected - 1);
            renderMenu(currentMenu);
            delay(200); 

            break;

        case EventType::JoyPressed:
            Serial.println("Pressed Joystick in Config State");
            if (currentMenu->entries[currentMenu->selected].subMenu != NULL) {
                currentMenu = currentMenu->entries[currentMenu->selected].subMenu;
                delay(200); 
            } else if (currentMenu->entries[currentMenu->selected].action != NULL) {
                currentMenu->entries[currentMenu->selected].action();
                delay(200);
            }
            renderMenu(currentMenu);
            break;

        case EventType::StartPulse:
            Serial.println("Transitioning to Running State");
            fsm->transitionTo(&runningState);
        default:
            break;
    }
}

void StateConfiguring::onEnter() {
    Serial.println("Entering Config state.");
    currentMenu->selected = 0;
    //renderMenu(currentMenu);
    // Additional entry logic, if needed
}

void StateConfiguring::onExit() {
    Serial.println("Exiting Config state.");
    // Additional exit logic, if needed
}
    