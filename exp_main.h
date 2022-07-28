typedef struct Option {
    char text[100];
    int y;         // Y coordinate
    int x;         // X coordinate
} Option;


Option* addOption(char* text, int y, int x) {
    Option *i = malloc(sizeof *i);
    strcpy(i->text, text);
    i->y = y;
    i->x = x;
    return i;
}

void printOption(WINDOW *win, const Option *self) {
    mvwprintw(win, (*self).y, (*self).x, (*self).text);
}

void printOptionCol(WINDOW *win, const Option *self) {
    wattron(win, A_REVERSE);
    printOption(win, self);
    wattroff(win, A_REVERSE);
}
