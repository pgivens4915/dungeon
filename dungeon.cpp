#include <ncurses.h>
#include "map.h"

int main(){
    // Declarations
    Map map;
    WINDOW *mainWindow;

    mainWindow = initscr();
    map.drawMap(mainWindow);

    refresh();
    // Waiting for input
    getch();
    endwin();

    return 0;
}
