#ifndef MENU_H
#define MENU_H
#include <stdlib.h>
#include <Arduino.h>

#define MAX_MENU_ITEMS 10

typedef struct MenuItem {
    std::string name;
    struct Menu* subMenu; // Pointer to sub-menu
    void (*action)();
    int* associatedValue;
} MenuItem;

typedef struct Menu {
    std::string name;
    MenuItem entries[MAX_MENU_ITEMS];
    int size;
    int selected;
} Menu;

void initDisplay();
void renderMenu(Menu* menu);

// Function prototypes
void startPulse();

extern Menu* currentMenu;
extern Menu Main;
extern Menu Settings;

#endif // MENU_H