
#include <Arduino.h>
#include "FSM/fsm.hpp"
#include "FSM/state_configuring.hpp"
#include "FSM/state_running.hpp"
#include "Event/event_queue.hpp"
#include "global_variables.hpp"

#include "Input/input.hpp"
#include "Menu/menu.hpp"

//Event queue declared in global_variables.hpp, structure in Event folder
EventQueue eventQueue;

//FSM structure placed in FSM folder
FSM* fsm;
StateConfiguring configState;
StateRunning runningState;


void setup() {
    Serial.begin(9600);
    fsm = new FSM(&configState, &eventQueue);

    initJoystick();
    initDisplay();

    renderMenu(currentMenu);
}

void loop() {
    fsm->update();

    updateJoystick();
    

    //delay(100); 
}
    