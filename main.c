#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include "main.h"

int counter();
void printTerm();
int flag = 0;
void setNewTime();
void readOldTime();

int main() {
    static struct termios oldt, newt;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    // setting terminal to accept input without enter
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
                else if (position == 1) setNewTime(&rep, &sesh, &brk);
                esc = 1;
                break;
            default :
                break;
        }
        if (esc == 1) break;

        system("clear");
        for (int i=0; i<n; i++) {
            for (int i=0; i<(w.ws_col/2 - 7); i++) { printf(" "); }
            (i != position) ? printOption(optionMenu[i])
                            : printOptionCol(optionMenu[i]);
        }
    } while ((sett=getchar()) != 'q');


    system("mpv sounds/start_sound.wav > /dev/null 2>&1 & disown");
    system("mpv sounds/start_sound.wav > /dev/null 2>&1 & disown");
    counter(rep, sesh, brk);
    system("mpv sounds/stop_sound.wav > /dev/null 2>&1 & disown");
    system("clear");

    // reseting terminal to old settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n");
    exit(EXIT_SUCCESS);
}

void readOldTime(int *rep, int *sesh, int *brk) {
    FILE *file;
    char buffer[5][5];
    int line = 0;
    file = fopen("settings.txt", "r");
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
    file = fopen("settings.txt", "w");
    printf("How many reps, how long the sesh and break: ");
    scanf("%d %d %d", &rep, &sesh, &brk);
    fprintf(file, "%d\n%d\n%d", rep, sesh, brk);
    fclose(file);
}

// checks if state should be switched (break, study)
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
        while (now - time(NULL) == 0) { }
        printTerm(cur_rep, min, sec);
        ++sec;
    }
}


// prints onto terminal
void printTerm(int cur_rep, int min, int sec) {
    system("clear");
    if (flag == 0)
        printf("       Study %d\n", cur_rep + 1);
    else
        printf("         Break %d\n", cur_rep + 1);
    printf("================\n");
    printf("\t%.2d:%.2d", min, sec);
    printf("\n================\n");
}

