#include "art.h" 
#include <stdbool.h>
#include <ncurses.h>
#include "preprocessor.h"
#include "quiz.h"
#include "main-menu.h"

int getChoice(WINDOW* win, bool start) {
    bool choice = start;

    while (true) {
        int ch = getch();
        if (ch == ASCII_ENTER) return choice;

        switch (ch) {
            case KEY_LEFT:
                choice = true;
                wmove(win, 2, 5);
                break;
            case KEY_RIGHT:
                choice = false;
                wmove(win, 2, 34);
                break;
            case 'q':
                return EXIT;
        } 
        wrefresh(win);
    }
}

WINDOW* loadAnswerBox(bool start) {
    WINDOW* local_win = createNewWin(5, 45, 14, 5);

    mvwprintw(local_win, 2, 5, "True");
    mvwprintw(local_win, 2, 34, "False");

    if (start == true) wmove(local_win, 2, 5);
    else wmove(local_win, 2, 34);

    wrefresh(local_win);

    return local_win;
}

bool playQuiz() {
    clear();
    printw("%s", quiz_header);
    refresh();

    WINDOW* instructions = newwin(3, 80, 10, 5);
    wprintw(instructions, "Welcome to the quiz! Once you press any key, questions will start to appear.\nUse the arrow keys to move between True and False, then press ENTER to select\nyour answer. If you want to go back to the menu at any point, just press 'q'.");
    wrefresh(instructions);
    getch();

    const bool correct_answers[4] = { true, false, true, true };
    int decision = true;

    for (int i = 0; i < 4; i++) {
        wclear(instructions);
        wprintw(instructions, "%s", questions[i]);
        wrefresh(instructions);

        WINDOW* win = loadAnswerBox(decision);
        decision = getChoice(win, decision);
        
        if (decision == EXIT )
            return FAIL;

        if (decision != correct_answers[i]) {
            wclear(instructions);
            wprintw(instructions, "Aww, looks like you got that one wrong. You'll have to try again.\nBetter luck next time! Press any key to go back to the menu.");
            wrefresh(instructions);
            getch();
            return FAIL;
        }
    }
    wclear(instructions);
    wprintw(instructions, "You passed, great job! The penguin should let you see Jason's true self now.\nPress any key to go back to the menu.");
    wrefresh(instructions);
    getch();

    return PASS;
}

const char* questions[4] = {
"Question 1: Philosophy rocks!",
"Question 2: Zeno of Elea knew what he was talking about.",
"Question 3: Ms. Bourke is one of Jason's top 4 favourite teachers this semester.",
"Question 4: You're going to give this project a good mark."
};
