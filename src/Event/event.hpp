
#pragma once

enum class EventType {
    None,
    JoyLeft,
    JoyRight,
    JoyUp,
    JoyDown,
    JoyPressed,
    StartPulse,
};

struct Event {
    EventType type;
    int value; 
};
    