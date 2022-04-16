#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <unistd.h>

// kbhit only in conio.h? not anymore
int kbhit() {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STD_FILENO is 0
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

int hour=0;minute=0;second=0;flag=0;cur_rep=1;rep_amnt=2;

void delay(ms) {
    clock_t timeDelay = ms + clock();
    while(timeDelay > clock());
}

int counter() {
    while(!kbhit() && (cur_rep - 1) != rep_amnt) {
        if (minute > 24 && flag == 0) {
            system("mpv bell_sound.wav > /dev/null 2>&1 & disown");
            minute = 0;
            flag = 1;
        } else if (minute > 4 && flag == 1) {
            cur_rep++;
            if ((cur_rep - 1) != rep_amnt) { system("mpv start_sound.wav > /dev/null 2>&1 & disown"); }
            minute = 0;
            flag = 0;
        }
        if (second > 59) {
            second = 0;
            ++minute;
        }
        printData();
        delay(1000000);
        ++second;
    }
}

int printData() {
    system("clear");
    if (flag == 0)
        printf("       Study %d\n", cur_rep);
    else
        printf("         Break %d\n", cur_rep);
    printf("===============\n");
    printf(" %d:%d:%d",hour,minute,second);
    printf("\n===============\n");
}

int main() {
    system("mpv start_sound.wav > /dev/null 2>&1 & disown"); // push to the background
    counter();
    system("mpv stop_sound.wav > /dev/null 2>&1 & disown");
    system("clear");
    printf("good sesh lad\n");
}
