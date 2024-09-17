#ifndef MENU_H
#define MENU_H
#include <stdlib.h>
#include <Arduino.h>

#define MAX_MENU_ITEMS 10

typedef struct MenuItem {
    char* name;
    struct Menu* subMenu; // Pointer to sub-menu
    void (*action)();
} MenuItem;

typedef struct Menu {
    const char* name;
    MenuItem entries[MAX_MENU_ITEMS];
    int size;
    int selected;
} Menu;

// Function prototypes
void startPulse();

// extern MenuItem forceSettingsEntries[];
// extern MenuItem mSettings[];
// extern MenuItem mMain[];
extern Menu* currentMenu;

#endif // MENU_H