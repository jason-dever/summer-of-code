#include "art.h"
#include <ncurses.h>
#include "preprocessor.h"

const int menu_column = 18;
const int penguin_column = 29;

WINDOW* createNewWin(int nav_box_height, int nav_box_width, int starty, int startx) {
    WINDOW* local_win = newwin(nav_box_height, nav_box_width, starty, startx);
    box(local_win, 0, 0);
    wrefresh(local_win);

    return local_win;
}

void displayNavBox(WINDOW* win, int start_y) {
    box(win, 0, 0);

    mvwprintw(win, 1, 1, "Instructions");
    mvwprintw(win, 2, 1, "Past Self");
    mvwprintw(win, 3, 1, "Academic Self");
    mvwprintw(win, 4, 1, "Future Self");
    mvwprintw(win, 5, 1, "True Self");
    wmove(win, start_y, 1);

    refresh();
}

void loadMainMenu() {
    erase();

    printw("%s", menu_header);
    mvprintw(menu_column, 0, "%s", menu_advice);
    mvprintw(penguin_column, 0, "%s", penguin);
    refresh();
}

int loopMainMenu(int start_y) {
    const int nav_box_height = 7;
    const int nav_box_width = 15;

    const int win_x = 3;
    const int win_y = 17;

    WINDOW* win = createNewWin(nav_box_height, nav_box_width, win_y, win_x);
    displayNavBox(win, 1);
    wmove(win, start_y, 1);
    wrefresh(win);

    int cursor_x = 1;
    int cursor_y = start_y;

    int ch = getch();
    while (ch != 'q') {
        switch (ch) {
            case KEY_UP:
                if (cursor_y > 1) {
                    cursor_y--; 
                }
                break;
            case KEY_DOWN: 
                if (cursor_y < nav_box_height-2) {
                    cursor_y++; 
                }
                break;
            case ASCII_ENTER:
                return cursor_y;
       } 
        wmove(win, cursor_y, cursor_x);
        wrefresh(win);
        ch = getch();
    }
    return EXIT;
}
