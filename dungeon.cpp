#include <ncurses.h>
#include "map.h"
#include "creature.h"
#include <list>


FILE* log;
int main(){
  // Declarations
  int gameStep(WINDOW* mainWindow, Map map);
  Map map;
  WINDOW *mainWindow;
  int retval;

  retval = map.initMap();
  if(retval != 0){
    return(1);
  }
  map.addCreature(3, 3, 'r');
  log = fopen("log.txt", "w");
  mainWindow = initscr();

  fprintf(log, "DEBUG\n");// DEBUG
  
  for(;;){
    gameStep(mainWindow, map);
  }
 
  endwin();

  return 0;
}

// Takes a single step of the game
int gameStep(WINDOW* mainWindow, Map map){
  std::list<Creature*>::iterator i;
  int retVal;
  char c;

  // Draw everything
  map.drawMap(mainWindow);
  // Draw all the creatures
  map.drawMoveCreatures(mainWindow);
  refresh();
  // Waiting for input
  do {
      c = getch();
  } while (c != '.');

}
