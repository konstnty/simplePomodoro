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

void textcolor(int attr, int fg) {
    char command[13];

    sprintf(command, "%c[%d;%dm", 0x1B, attr, fg + 30);
    printf("%s", command);
}
#endif
