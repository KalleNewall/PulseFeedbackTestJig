
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
    delay(100);
    Serial.begin(9600);
    delay(100);
    esp_log_level_set("*", ESP_LOG_NONE);
    esp_log_level_set("ESP32PWM", ESP_LOG_NONE); 

    initJoystick();
    initDisplay();
    
    
    fsm = new FSM(&configState, &eventQueue);

    renderMenu(currentMenu);
}

void loop() {
    fsm->update();

    updateJoystick();
}
    