#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int counter();
void printTerm();
int flag = 0;
void setNewTime();
void readOldTime();

int main() {
    int rep_amnt , sesh_length , brk_amnt, sett;
    printf("1) start with last used time\n2) set new time\n3) set timew integration [ON\\OFF]\xAInput: ");
    scanf("%d", &sett);
    switch(sett) {
        case 1 :
            readOldTime(&rep_amnt, &sesh_length, &brk_amnt);
            break;
        case 2 :
            setNewTime();
            break;
        case 3 :
            //to implement
            break;
        default :
            printf("You weren't suppose to get here you know?\n");
            return 1;
    }


    system("mpv sounds/start_sound.wav > /dev/null 2>&1 & disown");
    system("mpv sounds/start_sound.wav > /dev/null 2>&1 & disown");
    counter(rep_amnt, sesh_length, brk_amnt);
    system("mpv sounds/stop_sound.wav > /dev/null 2>&1 & disown");
    system("clear");
    return 0;
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
