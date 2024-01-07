#include "art.h"
#include "main-menu.h"
#include <ncurses.h>
#include "preprocessor.h"
#include <stdbool.h>
#include <stdio.h>
#include "quiz.h"
#include "actual-work.h"

bool visited_past = false;
bool visited_academic = false;
bool visited_future = false;

bool quiz_passed = false;

int main() {
    initscr();
    cbreak();
    keypad(stdscr, true);
    scrollok(stdscr, true);
    noecho();

    loadMainMenu();
    int user_decision = loopMainMenu(1);
    while (true) {
        const int nav_box_height = 7;
        const int nav_box_width = 15;

        const int win_x = 3;
        const int win_y = 17;

        WINDOW* win = createNewWin(nav_box_height, nav_box_width, win_y, win_x);

        if (user_decision == EXIT) {
            break;
        }
        else {
            switch (user_decision) {
                case 1: 
                    mvprintw(menu_column, 0, "%s", menu_advice);
                    break;
                case PAST_SELF:
                    visited_past = true;
                    mvprintw(menu_column, 0, "%s", past_self_segue);

                    displayNavBox(win, user_decision);
                    wrefresh(win);
                    getch();

                    clear();
                    printw("%s", past_self_info);
                    move(0, 0);
                    refresh();
                    while (getch() != 'q') { }

                    loadMainMenu();
                    break;
                case ACADEMIC_SELF:
                    if (visited_past) {
                        visited_academic = true;
                        mvprintw(menu_column, 0, "%s", academic_self_segue);

                        displayNavBox(win, user_decision);
                        wrefresh(win);
                        getch();

                        clear();
                        printw("%s", academic_self_info);
                        move(0, 0);
                        refresh();
                        while (getch() != 'q') { }
                        
                        loadMainMenu();
                        break;
                    }
                    else {
                        mvprintw(menu_column, 0, "%s", academic_self_not_allowed);
                    }
                    break;
                case FUTURE_SELF:
                    if (visited_academic) {
                        visited_future = true;
                        mvprintw(menu_column, 0, "%s", future_self_segue);

                        displayNavBox(win, user_decision);
                        wrefresh(win);
                        getch();

                        clear();
                        printw("%s", future_self_info);
                        move(0, 0);
                        refresh();
                        while (getch() != 'q') { }

                        loadMainMenu();
                        break;
                    }
                    else {
                        mvprintw(menu_column, 0, "%s", future_self_not_allowed);
                    }
                    break;
                case TRUE_SELF:
                    if (visited_future) {
                        if (quiz_passed) {
                            mvprintw(menu_column, 0, "%s", true_self_segue);

                            displayNavBox(win, user_decision);
                            wrefresh(win);
                            getch();

                            clear();
                            printw("%s", true_self_info);
                            move(0, 0);
                            refresh();
                            while (getch() != 'q') { }

                            loadMainMenu();
                            break;
                        }
                        else {
                            mvprintw(menu_column, 0, "%s", true_self_quiz_segue);

                            displayNavBox(win, user_decision);
                            wrefresh(win);

                            getch();

                            quiz_passed = playQuiz();
                            loadMainMenu();
                        }
                    }
                    else {
                        mvprintw(menu_column, 0, "%s", true_self_not_allowed);
                    }
                    break;
            }
            displayNavBox(win, 1);
            refresh();
        }
        user_decision = loopMainMenu(user_decision);
    }
    endwin();
    return 0;
}
