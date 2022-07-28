#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <string.h>
#include "exp_main.h"

int counter();
int flag = 0;
void printTerm();
void setNewTime();
void readOldTime();
Option* addOption();
void printOption();


int main() {
    initscr();
    noecho();
    curs_set(0);

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *menu = newwin(yMax/2, xMax/2, yMax/4, xMax/4);
    keypad(menu, true);
    box(menu, 0, 0);

/*    mvwprintw(menu, 1, 1, "start with last used time");
    mvwprintw(menu, 3, 1, "start with a new time");
    mvwprintw(menu, 5, 1, "toggle timew integration");
*/
    Option** optionMenu = malloc(3 * sizeof(Option*));
    optionMenu[0] = addOption("start with last used time", 2, 2);
    optionMenu[1] = addOption("start with a new time", 3, 2);
    optionMenu[2] = addOption("toggle timew integration", 4, 2);

    for (int i=0; i<3; i++) {
        printOption(menu, optionMenu[i]);
    }

    int rep_amnt, sesh_length, brk_amnt;
    int position = -1;
    char ch;
    while ((ch = wgetch(menu)) != 'q') {
        switch (ch) {
            case 'j':
                (position == 2) ? position = 2 : position++;
                break;
            case 'k':
                (position == 0) ? position = 0 : position--;
                break;
            default:
                break;
        }

        for (int i=0; i<3; i++) {
            (i != position) ? printOption(menu, optionMenu[i]) : printOptionCol(menu, optionMenu[i]);
        }
    }

    system("mpv sounds/start_sound.wav > /dev/null 2>&1 & disown");
    system("mpv sounds/start_sound.wav > /dev/null 2>&1 & disown");
    counter(rep_amnt, sesh_length, brk_amnt);
    system("mpv sounds/stop_sound.wav > /dev/null 2>&1 & disown");
    system("clear");

    endwin();
    return 0;
}

void readOldTime(int *rep, int *sesh, int *brk) {
    FILE *file;
    char buffer[5][5];
    int line = 0;
    file = fopen("exp_settings.txt", "r");
    while (!feof(file) && !ferror(file))
        if (fgets(buffer[line], 5, file) != NULL)
            line++;
    *rep = atoi(buffer[0]);
    *sesh = atoi(buffer[1]);
    *brk = atoi(buffer[2]);
    fclose(file);
}

void setNewTime(int *rep, int *sesh, int *brk) {
    FILE *file;
    file = fopen("exp_settings.txt", "w");
    printf("How many reps, how long the sesh and break: ");
    scanf("%d %d %d", &rep, &sesh, &brk);
    fprintf(file, "%d\n%d\n%d", rep, sesh, brk);
    fclose(file);
}

// checks if state should be switched (break, study)
int counter(int rep, int sesh, int brk) {
    int yMax, xMax, cur_rep = 0,  min = 0, sec = 0;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW *timer = newwin(yMax/2, xMax/2, yMax/4, xMax/4);
    box(timer, 0, 0);
    while (cur_rep != rep) {
        time_t now = time(NULL);

        if (min >= sesh && flag == 0) {
            min = 0;
            flag = 1;
            system("mpv sounds/bell_sound.wav > /dev/null 2>&1 & disown");
        } else if (min >= brk && flag == 1) {
            ++cur_rep;
            min = 0;
            flag = 0;
            if (cur_rep != rep) { system("mpv sounds/start_sound.wav >/dev/null 2>&1 & disown"); }
        }
        if (sec > 59) {
            sec = 0;
            ++min;
        }
        while (now - time(NULL) == 0) { }
        printTerm(timer, cur_rep, min, sec);
        ++sec;
    }
}

void printTerm(WINDOW *win, int cur_rep, int min, int sec) {
    wrefresh(win);
    if (flag == 0)
        mvwprintw(win, 1, 1, "Study %d\n", cur_rep + 1);
    else
        mvwprintw(win, 1, 1, "Break %d\n", cur_rep + 1);
    mvwprintw(win, 2, 1, "================");
    mvwprintw(win, 3, 1, "\t%.2d:%.2d", min, sec);
    mvwprintw(win, 4, 1, "================");
}

