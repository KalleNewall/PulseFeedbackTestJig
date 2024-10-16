#include "input.hpp"
#include <Arduino.h>
#include "Event/event.hpp"
#include "global_variables.hpp"

void initJoystick(){
    pinMode(joyButtonPin, INPUT_PULLUP);
}

void updateJoystick(){
    if (analogRead(joyXPin)>2500) {
        eventQueue.push({ EventType::JoyLeft, 0 });
    }
    if (analogRead(joyXPin)<1000) {
        eventQueue.push({ EventType::JoyRight, 0 });
    }
    if (analogRead(joyYPin)>2500) {
        eventQueue.push({ EventType::JoyDown, 0 });
    }
    if (analogRead(joyYPin)<1000) {
        eventQueue.push({ EventType::JoyUp, 0 });
    }
    if (digitalRead(joyButtonPin) == LOW){
        eventQueue.push({ EventType::JoyPressed, 0});
    }
}