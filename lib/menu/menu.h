#ifndef MENU_H
#define MENU_H

typedef struct MenuItem {
    const char* name;
    struct Menu* subMenu;
    void (*action)();
} MenuItem;

typedef struct Menu {
  char* name; //For debugging
  int size;
  int selected;
  MenuItem selectedEntry;
  struct Menu* parentMenu; // Added struct keyword for clarity
  MenuItem entries[];
} Menu;

// extern MenuItem forceSettingsEntries[];
// extern MenuItem mSettings[];
// extern MenuItem mMain[];
extern Menu* currentMenu;


void initializeMenus();
void updateVariable1();
void startPulse();

#endif // MENU_H