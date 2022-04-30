#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <unistd.h>

// kbhit only in conio.h? not anymore
// tbh still some problems with, it catches the input but doesnt act on it
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

int hour=0;minute=0;second=0;flag=0;
int wrk_amnt;brk_amnt;cur_rep=0;rep_amnt;tmw;

void delay(int ms) {
    clock_t timeDelay = ms + clock();
    while(timeDelay > clock());
}

int counter() {
    while(!kbhit() && (cur_rep) != rep_amnt) {
        if (minute > wrk_amnt && flag == 0) {
            minute = 0;
            flag = 1;
            system("mpv bell_sound.wav > /dev/null 2>&1 & disown");
            if (tmw == 1) { system("timew stop > /dev/null"); }
        } else if (minute > brk_amnt && flag == 1) {
            ++cur_rep;
            minute = 0;
            flag = 0;
            if ((cur_rep) != rep_amnt) { system("mpv start_sound.wav > /dev/null 2>&1 & disown"); }
            if (tmw == 1) { system("timew start uni > /dev/null"); }
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
        printf("       Study %d\n", cur_rep + 1);
    else
        printf("         Break %d\n", cur_rep + 1);
    printf("===============\n");
    printf(" %d:%d:%d",hour,minute,second);
    printf("\n===============\n");
}

int main() {
    printf("Rep amount: ");
    scanf("%d", &rep_amnt);
    printf("Work length: ");
    scanf("%d", &wrk_amnt);
    printf("Break length: ");
    scanf("%d", &brk_amnt);
    printf("Timew integration? ");
    scanf("%d", &tmw);

    system("mpv start_sound.wav > /dev/null 2>&1 & disown");
    if (tmw == 1) {  system("timew start uni > /dev/null"); }
    counter();
    system("mpv stop_sound.wav > /dev/null 2>&1 & disown");
    if (tmw == 1) { system("timew stop > /dev/nyll"); }
    system("clear");
    printf("good sesh lad\n");
}
