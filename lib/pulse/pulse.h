#ifndef PULSE_H
#define PULSE_H

#include <Servo.h>
#include "global_variables.h"

class Pulse {
public:
    Pulse(int pin);
    Pulse();
    void moveServo(bool drivePulse);
    void attachServo(int pin);

private:
    Servo myServo;
    int pos;
};

#endif