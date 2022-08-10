#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include "main.h"
#include "kbhit.h"

int counter();
void printTerm();
int flag = 0;

int main() {
    static struct termios oldt, newt;

    /* setting terminal to accept input without enter */
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int n=2, num=1;
    Option** optionMenu = malloc(n * sizeof(Option*));
    optionMenu[0] = addOption("last used time\n");
    optionMenu[1] = addOption("new time\n");
//     optionMenu[2] = addOption("toggle timew integration\n");

    int rep, sesh, brk;
    int position = 0;
    int sett, esc = 0;
    do {
        switch(sett) {
            case 'j' :
                (position == n-1) ? position = n-1 : position++;
                break;

            case 'k' :
                (position == 0) ? position = 0 : position--;
                break;

            case '\n' :
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                if (position == 0) readOldTime(&rep, &sesh, &brk);
                else if (position == 1) {
                    printf("How many reps, how long the sesh and break: ");
                    scanf("%d %d %d", &rep, &sesh, &brk);
                    setNewTime(rep, sesh, brk);
                }

                system("mpv sounds/start_sound.wav > /dev/null 2>&1 & disown");
                system("mpv sounds/start_sound.wav > /dev/null 2>&1 & disown");
                counter(rep, sesh, brk);
                system("mpv sounds/stop_sound.wav > /dev/null 2>&1 & disown");
                system("clear");
                tcsetattr(STDIN_FILENO, TCSANOW, &newt);

            default :
                break;
        }
        system("clear");

        /* creates struct with window size */
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        for (int i=0; i<n; i++) {
            for (int i=0; i<(w.ws_col/2 - 7); i++) { printf(" "); }
            (i != position) ? printOption(optionMenu[i])
                            : printOptionCol(optionMenu[i]);
        }
    } while ((sett=getchar()) != 'q');



    /* reseting terminal to old settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n");
    exit(EXIT_SUCCESS);
}

/* checks if state should be switched (break, study) */
int counter(int rep, int sesh, int brk) {
    int cur_rep = 0; int min = 0, sec = 0;
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

        while (now - time(NULL) == 0) {
            if(!kbhit()) continue;

            switch(getchar()) {
                case 'q':
                    cur_rep = rep;
                    break;
                case 'p':
                    printf("Paused ");
                    while(getchar() != 'p') {  }
                    break;
            }

        }
        printTerm(cur_rep, min, sec);
        ++sec;
    }
}


/* prints to terminal */
void printTerm(int cur_rep, int min, int sec) {
    system("clear");
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    for (int i=0; i<(w.ws_col/2 - 7); i++) { printf(" "); }
    if (flag == 0)
        printf("       Study %d\n", cur_rep + 1);
    else
        printf("         Break %d\n", cur_rep + 1);
    for (int i=0; i<(w.ws_col/2 - 7); i++) { printf(" "); }
    printf("================\n");
    for (int i=0; i<(w.ws_col/2 - 7); i++) { printf(" "); }
    printf("\t%.2d:%.2d", min, sec);
    printf("\n");
    for (int i=0; i<(w.ws_col/2 - 7); i++) { printf(" "); }
    printf("================\n");
}

