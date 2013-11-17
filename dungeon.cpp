#include <ncurses.h>
#include "map.h"
#include "creature.h"

int main(){
  // Declarations
  Map map;
  WINDOW *mainWindow;
  Creature rat(3,3,'r');
  int retval;

  retval = map.initMap();
  if(retval != 0){
    return(1);
  }

  mainWindow = initscr();
  map.drawMap(mainWindow);
  rat.drawCreature(mainWindow);

  refresh();
  // Waiting for input
  getch();
  endwin();

  return 0;
}
