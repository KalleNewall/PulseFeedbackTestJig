// menu.c
#include "menu.h"
#include <Arduino.h>

// Function implementations
void updateVariable1() {
    Serial.println("Selected Toggle Force Mode");
}

void startPulse() {
    Serial.println("Pulse started");
}

Menu* createMenu(int size, char* name, MenuItem* items, Menu* parent) {
    Menu* newMenu = (Menu*)malloc(sizeof(Menu) + sizeof(MenuItem) * (size));
    if (newMenu) {
        newMenu->name = name;
        newMenu->size = size;
        newMenu->parentMenu = parent;
        newMenu->selected = 0;
        for (int i = 0; i < size; i++) {
            newMenu->entries[i] = items[i];
        }
        newMenu->selectedEntry = newMenu->entries[0];
    }
    return newMenu;
}

Menu* ForceSettings;
Menu* Settings;
Menu* Main;


MenuItem itemsForceSettings[2] = {
    {"Toggle Force Mode", NULL, updateVariable1},
    {"Max Force", NULL, NULL}
};

//const int forceSettingsSize = sizeof(itemsForceSettings) / sizeof(MenuItem);

MenuItem itemsSettings[1]; 
//const int settingsSize = sizeof(itemsSettings) / sizeof(MenuItem);

MenuItem itemsMain[2]; 
//const int mainSize = sizeof(itemsMain) / sizeof(MenuItem);




void initializeMenus() {

    ForceSettings = createMenu(2, "Menu Force Settings", itemsForceSettings, NULL);

    itemsSettings[0] = {"Force Settings", ForceSettings, NULL}; 
    Settings = createMenu(1, "Menu Settings", itemsSettings, NULL); 

    itemsMain[0] = {"Start Pulse", NULL, startPulse}; 
    itemsMain[1] = {"Settings", Settings, NULL};
    Main = createMenu(2, "Menu Main", itemsMain, NULL); 

    Settings->parentMenu = Main; 
    ForceSettings->parentMenu = Settings; 
}



void printEntries(Menu* menu){
    for(int i = 0; i < menu->size; i++){
        Serial.println(menu->entries[i].name);
    }
}


