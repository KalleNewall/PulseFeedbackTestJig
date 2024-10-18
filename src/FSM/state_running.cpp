
#include "state_running.hpp"
#include <Arduino.h>
#include <ESP32Servo.h>
#include "state_configuring.hpp"
#include "fsm.hpp"
#include "global_variables.hpp"


Servo myServo;
extern FSM* fsm;
extern StateConfiguring configState;
extern EventQueue eventQueue;

enum ServoState {
  IDLE,
  MOVING_TO_POSITION,
  HOLDING_POSITION,
  RETURNING,
  WAITING_BPM
};

unsigned long previousMillis = 0; 
unsigned long interval = 0;        

ServoState currentState = IDLE;
//This should be moved to somewhere more fitting
unsigned long bpmToDelay(float bpm){
    // Add max bpm check?
    if (bpm <= 0) {
        return 0; 
    }
    return 60000 / bpm; 
}

void StateRunning::handle() {
    //Serial.println("Handling Running State...");
    unsigned long currentMillis = millis();

    if (!pulseVars.forceMode) {
    switch (currentState) {
      case IDLE:
        // Move to the specified distance
        myServo.write(pulseVars.distance);
        previousMillis = currentMillis;  // Record the current time
        interval = 100;  // Set interval to hold the position for 100ms
        currentState = MOVING_TO_POSITION;
        break;

      case MOVING_TO_POSITION:
        // Wait until 100ms has passed
        if (currentMillis - previousMillis >= interval) {
          // Return to zero position
          myServo.write(0);
          previousMillis = currentMillis;  // Reset the timer
          interval = bpmToDelay(pulseVars.BPM);  // Set interval based on BPM
          currentState = RETURNING;
        }
        break;

      case RETURNING:
        // Wait until the BPM delay has passed before starting the next pulse
        if (currentMillis - previousMillis >= interval) {
          currentState = IDLE;  // Go back to the idle state to start the next cycle
        }
        break;
    }
  }
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
    myServo.write(0);
}

void StateRunning::onExit() {
    Serial.println("Exiting Running state.");
    myServo.detach();
  
}
    