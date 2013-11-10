#include <ncurses.h>
#include "map.h"

int main(){
    // Declarations
    Map map;

    initscr();
    map.drawMap();

    refresh();
    getch();
    endwin();

    return 0;
}
