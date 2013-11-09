#include <ncurses.h>

int main(){
    initscr();
    mvprintw(10,10, "Hello World");

    refresh();
    getch();
    endwin();

    return 0;
}
