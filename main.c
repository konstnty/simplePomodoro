#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

int counter();
void printTerm();
int flag = 0;

int main() {
    int rep_amnt = 0, sesh_length = 0, brk_amnt = 0;

    printf("How many reps, how long the sesh and break: ");
    scanf("%d %d %d", &rep_amnt, &sesh_length, &brk_amnt);

    system("mpv start_sound.wav > /dev/null 2>&1 & disown");
    system("mpv start_sound.wav > /dev/null 2>&1 & disown");
    counter(rep_amnt, sesh_length, brk_amnt);
    system("mpv stop_sound.wav > /dev/null 2>&1 & disown");
    system("clear");
    return 0;
}

int counter(int rep, int sesh, int brk) {
    int cur_rep = 0; int min = 0, sec = 0;
    while (cur_rep != rep) {
        time_t now = time(NULL);

        if (min >= sesh && flag == 0) {
            min = 0;
            flag = 1;
            system("mpv bell_sound.wav > /dev/null 2>&1 & disown");
        } else if (min >= brk && flag == 1) {
            ++cur_rep;
            min = 0;
            flag = 0;
            if (cur_rep != rep) { system("mpv start_sound.wav >/dev/null 2>&1 & disown"); }
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
