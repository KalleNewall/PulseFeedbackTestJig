
#include "state_running.hpp"
#include <Arduino.h>
#include <ESP32Servo.h>
#include "state_configuring.hpp"
#include "fsm.hpp"
#include "global_variables.hpp"


Servo myServo;
extern FSM* fsm;
extern StateConfiguring configState;

//This should be moved to somewhere more fitting
unsigned long bpmToDelay(float bpm){
    // Add max bpm check?
    if (bpm <= 0) {
        return 0; 
    }
    return 60000 / bpm; 
}

void StateRunning::handle() {
    Serial.println("Handling Running State...");

    //if(!pulseVars.forceMode){
        int pos = 0;
        int increment = 1;
        myServo.write(pulseVars.distance);
        delay(100);
        myServo.write(0);
        delay(bpmToDelay(pulseVars.BPM));
    //}


    // for (pos = 0; pos <= pulseVars.distance; pos += increment) {
    //     myServo.write(pos);   
    //     delay(15);   
    // }

    
    // for (pos = pulseVars.distance; pos >= 0; pos -= increment) {
    //     myServo.write(pos);   
    //     delay(15);    
    // }
}

void StateRunning::handleEvent(const Event& event) {
    switch (event.type) {
        case EventType::JoyLeft:
            Serial.println("Transitioning to Config state...");
            fsm->transitionTo(&configState);
            break;

        default:
            break;
    }
}

void StateRunning::onEnter() {
    Serial.println("Entering Running state.");
    myServo.attach(servoPin, 900, 2000);  // Attach the servo to pin 18
    myServo.setPeriodHertz(50);
    // Additional entry logic, if needed
}

void StateRunning::onExit() {
    Serial.println("Exiting Running state.");
    myServo.detach();
    // Additional exit logic, if needed
}
    