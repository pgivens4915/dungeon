#include <ncurses.h>
#include "map.h"

int main(){
  // Declarations
  Map map;
  WINDOW *mainWindow;
  int retval;

  retval = map.initMap();
  if(retval != 0){
    return(1);
  }

  mainWindow = initscr();
  map.drawMap(mainWindow);

  refresh();
  // Waiting for input
  getch();
  endwin();

  return 0;
}
