// TestJig.ino
#include "menu.h"
#include "menu.cpp"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int joyXPin = A1;
const int joyYPin = A0;
const int joyButtonPin = 2;
const int pushButtonPin = 4;

bool joyButtonPressed = false;
bool pushButtonPressed = false;

int SSD1306_I2C_ADDRESS = 0x3C;

Menu* currentMenu;

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
    }
    display.display();
}

void setup() {
    Serial.begin(9600);
   
    if (!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    Serial.println("OLED Initialized");
    display.clearDisplay();
    display.setTextSize(1); // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text

    
    pinMode(joyButtonPin, INPUT_PULLUP);
    pinMode(pushButtonPin, INPUT_PULLUP);

    initializeMenus();

    currentMenu = Main;


}

void loop() {
    int joyX = analogRead(joyXPin);
    int joyY = analogRead(joyYPin);
    int joyButtonState = digitalRead(joyButtonPin);
    int pushButtonState = digitalRead(pushButtonPin);

    
    if (joyY < 400) { 
        currentMenu->selected = max(0, currentMenu->selected - 1);
        currentMenu->selectedEntry = currentMenu->entries[currentMenu->selected];
        delay(200); 
    } else if (joyY > 600) { 
        currentMenu->selected = min(currentMenu->size - 1, currentMenu->selected + 1);
        currentMenu->selectedEntry = currentMenu->entries[currentMenu->selected];
        delay(200); 
    }


    if (joyButtonState == LOW && !joyButtonPressed) {
        if (currentMenu->entries[currentMenu->selected].subMenu != NULL) {
            currentMenu = currentMenu->selectedEntry.subMenu;
            delay(200); // Debounce
        } else if (currentMenu->entries[currentMenu->selected].action != NULL) {
            currentMenu->entries[currentMenu->selected].action();
            delay(200); // Debounce
        }
    } else if (joyButtonState == HIGH && joyButtonPressed) {
        joyButtonPressed = false;
    }

    renderMenu(currentMenu);

    if (pushButtonState == LOW && !pushButtonPressed) { 
        Serial.println(currentMenu->parentMenu->name);
        if (currentMenu->parentMenu != NULL) {
            currentMenu = currentMenu->parentMenu;
            delay(200); // Debounce delay
        }
    } else if (pushButtonState == HIGH && pushButtonPressed){
        pushButtonPressed = false;
    }
}

