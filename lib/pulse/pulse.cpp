#include "Pulse.h"
#include <Arduino.h>


Pulse::Pulse(int pin) {
    myServo.attach(pin);
    pos = 0;
}

Pulse::Pulse(){
    ; //Weird but works
}

void Pulse::attachServo(int pin){
    myServo.attach(pin);
    pos = 0;
}

unsigned long bpmToDelay(float bpm){
    // Add max bpm check?
    if (bpm <= 0) {
        return 0; 
    }
    return 60000 / bpm; 
}

void Pulse::moveServo(bool drivePulse) {
    if (drivePulse) {
        myServo.write(pulseDistance);
        delay(100);
        myServo.write(0);
        delay(bpmToDelay(BPM));
    }
}