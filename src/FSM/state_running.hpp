
#pragma once
#include "fsm_state.hpp"

const int servoPin = 18;

class StateRunning : public FSMState {
public:
    void handle() override;
    void handleEvent(const Event& event) override;
    void onEnter() override; 
    void onExit() override; 
private:
    void driveServoBloodPressureCurve();  // Blood pressure curve mode
    unsigned long previousMillis = 0;
    unsigned long resetTime = 0;
    int dataPointIndex = 0;
    int beatsPerMinute = 30;  // Heart rate
    unsigned long timeBetweenSetpointUpdates;

    const int numDataPoints = 45;
    const double pressureData[45] = {
        71.033, 72.119, 75.493, 78.048, 86.125, 88.578, 91.032, 93.384, 95.633, 97.474,
        99.52, 99.419, 98.5, 96.763, 95.333, 94.21, 94.211, 91.963, 89.408, 86.955,
        84.399, 82.355, 84.809, 87.364, 89.716, 89.104, 89.616, 88.698, 87.472, 86.656,
        85.328, 83.591, 82.877, 81.447, 80.324, 79.61, 78.384, 77.67, 76.751, 75.628,
        74.3, 72.972, 71.645, 70.726, 71.033
    }; 
};
    