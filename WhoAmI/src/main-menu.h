#pragma once

#include <ncurses.h>

WINDOW* createNewWin(int, int, int, int);
void displayNavBox(WINDOW*, int);

void loadMainMenu();
int loopMainMenu(int);

extern const int penguin_column;
extern const int menu_column;
