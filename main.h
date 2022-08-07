#ifndef MAIN_H
#define MAIN_H

#define RESET       0
#define BRIGHT 		1
#define DIM		    2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

#define BLACK 		0
#define RED		    1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7


/* prinitng colored output on linux by adding escape codes before string */
void textcolor(int attr, int fg) {
    char command[13];

    sprintf(command, "%c[%d;%dm", 0x1B, attr, fg + 30);
    printf("%s", command);
}


/* option struct and print functions */
typedef struct Option {
    char text[100];
} Option;


Option* addOption(char* text) {
    Option *i = malloc(sizeof *i);
    strcpy(i->text, text);
    return i;
}

void printOption(const Option *self) {
    printf(self->text);
}

void printOptionCol(const Option *self) {
    textcolor(BRIGHT, RED);
    printOption(self);
    textcolor(RESET, WHITE);
}

/* reading and saving to settings.txt */
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

#endif
