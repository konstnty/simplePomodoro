#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include "main.h"
#include "kbhit.h"

void counter(int,int,int,uint8_t);
void printTerm();
int flag = 0;

/* creates struct with window size */
struct winsize w;

/* defining options */
#define SAVE        0x01    /* 0b00000001 */
#define TIMEW_INTEG 0x02    /* 0b00000010 */

int main() {
    static struct termios oldt, newt;

    /* setting terminal to accept input without enter */
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int n=4;
    Option** optionMenu = malloc(n * sizeof(Option*));
    optionMenu[0] = addOption("last used time\n");
    optionMenu[1] = addOption("new time\n");
    optionMenu[2] = addOption("toggle save/tags\n");
    optionMenu[3] = addOption("toggle timew integration\n");

    uint8_t option;
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
                switch(position) {
                    case 0:
                        readOldTime(&rep, &sesh, &brk);
                        break;
                    case 1:
                        for (int i=0; i<(w.ws_col/2 - 7); i++) { printf(" "); }
                        printf("How many reps, how long the sesh and break: ");
                        scanf("%d %d %d", &rep, &sesh, &brk);
                        setNewTime(rep, sesh, brk);
                        break;
                    case 2:
                        option ^= SAVE;
                        break;
                    case 3:
                        option ^= TIMEW_INTEG;
                        break;
                    default:
                        break;
                }
                tcsetattr(STDIN_FILENO, TCSANOW, &newt);

                if (position != 0 && position != 1) { break; }
                counter(rep, sesh, brk, option);

            default :
                break;
        }
        system("clear");

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
void
counter(int rep, int sesh, int brk, uint8_t option) {
    int cur_rep = 0; int min = 0, sec = 0;
    system("mpv sounds/start_sound.wav > /dev/null 2>&1 & disown");
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
                    for (int i=0; i<(w.ws_col/2 - 7); i++) { printf(" "); }
                    printf("Paused ");
                    while(getchar() != 'p')
                        ;
                    break;
            }

        }

        printTerm(cur_rep, min, sec);
        ++sec;
    }
    system("mpv sounds/stop_sound.wav > /dev/null 2>&1 & disown");
    system("clear");
}


/* prints to terminal */
void
printTerm(int cur_rep, int min, int sec) {
    system("clear");
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
