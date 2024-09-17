// menu.c
#include "menu.h"
#include "pulse.h"
#include "global_variables.h"
#include <Arduino.h>
//#include <avr/pgmspace.h>

// Function implementations
void toggleForceMode() {
    if(forceModeActive){
        forceModeActive = false;
    } else{
        forceModeActive = true;
    }
    Serial.print("Force mode: ");
    Serial.println(forceModeActive);
}

void setDistance(){
    pulseDistance += 10;
    if(pulseDistance > 90){
        pulseDistance = 10;
    }
    Serial.print("Pulse distance: ");
    Serial.println(pulseDistance);
}

void setBpm(){
    BPM += 10;
    if(BPM > 100){
        BPM = 20;
    }
    Serial.print("BPM: ");
    Serial.println(BPM);
}

void setForce(){
    maxForce += 10;
    if(maxForce > 100){
        maxForce = 10;
    }
    Serial.print("Max Force: ");
    Serial.println(maxForce);
}

void updateVariable2() {
    Serial.println("Changing BPM");
}

void startPulse() {
    Serial.println("Pulse started");
    drivePulse = true;
}

Menu Settings = {
    "Settings Menu",
    {
        {"BPM", NULL, setBpm},
        {"Distance", NULL, setDistance},
        {"Force Mode", NULL, toggleForceMode},
        {"Max Force", NULL, setForce},
    },
    4, // Number of entries
    0  // Initially selected entry
};


// Example main menu
Menu Main = {
    "Main Menu",
    {
        {"Start Pulse", NULL, startPulse},
        {"Settings", &Settings, NULL}, // Link to sub-menu
    },
    2, // Number of entries
    0  // Initially selected entry
};


