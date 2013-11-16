#include <ncurses.h>
#include "map.h"

int main(){
    // Declarations
    Map map;
    WINDOW *mainWindow;

    initscr();
    map.drawMap();

    refresh();
    getch();
    endwin();

    return 0;
}
