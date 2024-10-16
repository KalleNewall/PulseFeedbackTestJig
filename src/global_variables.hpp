#pragma once
#include "Event/event_queue.hpp"

extern EventQueue eventQueue;

extern struct PulseVariables{
    int distance = 10;
    int BPM = 60;
    int forceMode = 0;
    int maxForce = 20;
}pulseVars;



