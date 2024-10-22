
#include "state_running.hpp"
#include <Arduino.h>
#include <ESP32Servo.h>
#include "state_configuring.hpp"
#include "fsm.hpp"
#include "global_variables.hpp"
#include "Force/QwiicScale.hpp"

Servo myServo;
QwiicScale myScale;

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
unsigned long pulseWaitTime = 0;        

int currentServoPosition = 0;  // Track the current servo position
int stepSize = 1;              // Servo movement step size

ServoState currentServoState = IDLE;
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

    myScale.update();
    float currentForce = myScale.getAverageWeight();  
    
    switch (currentServoState) {
      case IDLE:
        // Start moving the servo to the target position in small steps
        currentServoPosition = 0;  // Start from 0 degrees
        previousMillis = currentMillis;  // Record the current time
        pulseWaitTime = 100;  // Set pulseWaitTime for step movement (50 ms per step)
        currentServoState = MOVING_TO_POSITION;
        break;

      case MOVING_TO_POSITION:
        // Move the servo in steps and check the force
        //if (currentMillis - previousMillis >= pulseWaitTime) {
          // Move the servo by the step size
          currentServoPosition += stepSize;
          myServo.write(currentServoPosition);
          previousMillis = currentMillis;  // Reset the timer

          // If in force mode and the current force exceeds the max force, stop
          if (pulseVars.forceMode && currentForce > pulseVars.maxForce) {
              Serial.println("Force exceeded max force! Stopping servo.");
              Serial.print("Force was: ");
              Serial.println(currentForce);
              myServo.write(0); // Move back to start position
              previousMillis = currentMillis;  // Reset the timer
              pulseWaitTime = bpmToDelay(pulseVars.BPM);  // Set pulseWaitTime based on BPM
              currentServoState = RETURNING;  // Move to the RETURNING state
          }

          // If the target position is reached, stop moving forward
          if (currentServoPosition >= pulseVars.distance) {
              previousMillis = currentMillis;  // Reset the timer
              myServo.write(0);  // Move back to start position
              pulseWaitTime = bpmToDelay(pulseVars.BPM);  // Set pulseWaitTime based on BPM
              currentServoState = RETURNING;  // Move to the RETURNING state
          }
        //}
        break;

      case RETURNING:
        // Wait until the BPM delay has passed before starting the next pulse
        if (currentMillis - previousMillis >= pulseWaitTime) {
          currentServoState = IDLE;  // Go back to the idle state to start the next cycle
        }
        break;
    }
}

void StateRunning::handleEvent(const Event& event) {
    switch (event.type) {
        case EventType::JoyPressed:
            Serial.println("Transitioning to Config state...");
            fsm->transitionTo(&configState);
            break;

        default:
            break;
    }
}

void StateRunning::onEnter() {
    Serial.println("Entering Running state.");
    myServo.attach(servoPin, 900, 2000);  // Attach the servo to pin 18 with min and max pulse width in [us]
    myServo.setPeriodHertz(50); // Servo pwm frequency at 50 Hz
    myServo.write(0);

    myScale.begin();
    Serial.println("Qwiic scale initialized.");
}

void StateRunning::onExit() {
    Serial.println("Exiting Running state.");
    myServo.write(0);
    delay(100);
    myServo.detach();
  
}
    