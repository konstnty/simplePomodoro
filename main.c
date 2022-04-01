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

int hour=0;minute=0;second=0;flag=0;

void delay(ms) {
    clock_t timeDelay = ms + clock();
    while(timeDelay > clock());
}

int counter() {
    while(!kbhit() && flag == 0) {
        if (minute > 59) {
            minute = 0;
            ++hour;
        }
        if (second > 59) {
            second = 0;
            ++minute;
        }
        printData();
        delay(1000);
        ++second;
    }
}

int printData() {
    system("clear");
    printf("1.Start 2.Stop 3.Reset 4.End\n");
    printf("============================\n");
    printf("            %d:%d:%d",hour,minute,second);
    printf("\n============================\n");
}

int main() {
    counter();
    printData();
    return 0;
}
