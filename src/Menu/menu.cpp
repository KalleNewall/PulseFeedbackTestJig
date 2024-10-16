// menu.c
#include "menu.hpp"
//#include "pulse.h"
#include "global_variables.hpp"
#include <Arduino.h>
//#include <avr/pgmspace.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Defined in global_variables.hpp
PulseVariables pulseVars;

// Function implementations
void toggleForceMode() {
    pulseVars.forceMode = (pulseVars.forceMode == 1) ? 0 : 1; 
    Serial.print("Force mode: ");
    Serial.println(pulseVars.forceMode);
}

void setDistance(){
    pulseVars.distance += 5;
    if(pulseVars.distance > 30){
        pulseVars.distance = 10;
    }
    Serial.print("Pulse distance: ");
    Serial.println(pulseVars.distance);
}

void setBpm(){
    pulseVars.BPM += 10;
    if(pulseVars.BPM >= 120){
        pulseVars.BPM = 60;
    }
    Serial.print("BPM: ");
    Serial.println(pulseVars.BPM);
}

void setForce(){
    pulseVars.maxForce += 10;
    if(pulseVars.maxForce >= 100){
        pulseVars.maxForce = 10;
    }

    Serial.print("Max Force: ");
    Serial.println(pulseVars.maxForce);
}

void updateVariable2() {
    Serial.println("Changing BPM");
}

void startPulse() {
    Serial.println("Pulse started");
    eventQueue.push({ EventType::StartPulse, 0 });
}

Menu Settings = {
    "Settings Menu",
    {
        {"BPM", NULL, setBpm, &pulseVars.BPM},
        {"Distance", NULL, setDistance, &pulseVars.distance},
        {"Force Mode", NULL, toggleForceMode, &pulseVars.forceMode},
        {"Max Force", NULL, setForce, &pulseVars.maxForce},
    },
    4, // Number of entries
    0  // Initially selected entry
};



Menu Main = {
    "Main Menu",
    {
        {"Start Pulse", NULL, startPulse},
        {"Settings", &Settings, NULL}, 
    },
    2, // Number of entries
    0  // Initially selected entry
};

void renderMenu(Menu* menu) {
    display.clearDisplay();
    for (int i = 0; i < menu->size; i++) {
        display.setCursor(0, i * 10);
        if (i == menu->selected) {
            display.print("> "); 
            display.setTextColor(SSD1306_WHITE, SSD1306_BLACK); 
        } else {
            display.print("  "); 
            display.setTextColor(SSD1306_WHITE);
        }
        display.print(menu->entries[i].name);
        if(menu->entries[i].associatedValue != NULL){
            display.print(":");
            display.print(*menu->entries[i].associatedValue);
        }
    }
    display.display();
}

void initDisplay(){
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
}




