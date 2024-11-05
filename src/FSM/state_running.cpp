
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
int stepSize = 2;              // Servo movement step size

int servoStartPos = 120;

ServoState currentServoState = IDLE;
//This should be moved to somewhere more fitting
unsigned long bpmToDelay(float bpm){
    // Add max bpm check?
    if (bpm <= 0) {
        return 0; 
    }
    return 60000 / bpm; 
}

void StateRunning::driveServoBloodPressureCurve() {
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - resetTime;

    // Update the servo position to follow the pressure data points
    // if (dataPointIndex < numDataPoints - 1 && elapsedTime >= /*timeBetweenSetpointUpdates*/1) {
    dataPointIndex++;
    //resetTime = currentTime;  // Update the reset time
    if (dataPointIndex >= numDataPoints - 1) {
        // Reset the data point index to repeat the curve
        dataPointIndex = 0;
        delay(500);
        //resetTime = millis();
    }

    // Map the pressure data to a servo position 
    int servoPosition = map(pressureData[dataPointIndex], 70, 100, 0, 5);  

    // Move the servo to the calculated position
    myServo.write(servoPosition);

    // Debugging information for monitoring
    // Serial.print("Time: ");
    // Serial.print(millis());
    // Serial.print(", Pressure: ");
    // Serial.print(pressureData[dataPointIndex]);
    // Serial.print(", Servo Position: ");
    // Serial.println(servoPosition);
}

void StateRunning::handle() {
    //Serial.println("Handling Running State...");
    unsigned long currentMillis = millis();

    if(pulseVars.bloodPressureCurveMode){
      driveServoBloodPressureCurve();
      return;
    }
    myScale.update();
    float currentForce = myScale.getAverageWeight();  

    
    Serial.print("Current force: ");
    Serial.println(currentForce);
    switch (currentServoState) {

      case IDLE:
        // Start moving the servo to the target position in small steps
        currentServoPosition = servoStartPos;  // Start from 0 degrees
        previousMillis = currentMillis;  // Record the current time
        pulseWaitTime = 100;  // Set pulseWaitTime for step movement 
        currentServoState = MOVING_TO_POSITION;
        break;

      case MOVING_TO_POSITION:
        // Move the servo in steps and check the force
        //if (currentMillis - previousMillis >= pulseWaitTime) {
          // Move the servo by the step size
          currentServoPosition -= stepSize;
          myServo.write(currentServoPosition);
          previousMillis = currentMillis;  // Reset the timer

          // If in force mode and the current force exceeds the max force, stop
          if (pulseVars.forceMode && currentForce > pulseVars.maxForce) {
              Serial.println("Force exceeded max force! Stopping servo.");
              Serial.print("Force was: ");
              Serial.println(currentForce);
              myServo.write(servoStartPos); // Move back to start position
              previousMillis = currentMillis;  // Reset the timer
              pulseWaitTime = bpmToDelay(pulseVars.BPM);  // Set pulseWaitTime based on BPM
              currentServoState = RETURNING;  // Move to the RETURNING state
          }

          // If the target position is reached, stop moving forward
          if (currentServoPosition <= servoStartPos - pulseVars.distance) {
              previousMillis = currentMillis;  // Reset the timer
              myServo.write(servoStartPos);  // Move back to start position
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
    myServo.attach(servoPin, 700, 2100);  // Attach the servo to pin 18 with min and max pulse width in [us]
    myServo.setPeriodHertz(50); // Servo pwm frequency at 50 Hz
    myServo.write(servoStartPos);

    myScale.begin();
    Serial.println("Qwiic scale initialized.");
}

void StateRunning::onExit() {
    Serial.println("Exiting Running state.");
    myServo.write(servoStartPos);
    delay(100);
    myServo.detach();
  
}
    