#include <ncurses.h>
#include "map.h"
#include "creature.h"
#include <list>


FILE* log;
int main(){
  // Declarations
  int gameStep(WINDOW* mainWindow, std::list<Creature*> creatureList, Map map);
  Map map;
  WINDOW *mainWindow;
  Creature rat(3,3,'r');
  int retval;
  std::list<Creature*> creatureList;

  retval = map.initMap();
  if(retval != 0){
    return(1);
  }

  log = fopen("log.txt", "w");
  mainWindow = initscr();
  creatureList.push_front(&rat);

  fprintf(log, "DEBUG\n");// DEBUG

  for(;;){
    gameStep(mainWindow, creatureList, map);
  }
 
  endwin();

  return 0;
}

// Takes a single step of the game
int gameStep(WINDOW* mainWindow, std::list<Creature*> creatureList, Map map){
  std::list<Creature*>::iterator i;
  int retVal;

  // Move the creatures
  for(i = creatureList.begin(); i != creatureList.end(); ++i){
    retVal = (*i)->step();
    fprintf(log, "moving creature %i\n", retVal);// DEBUG
  }
  
  // Draw everything
  map.drawMap(mainWindow);
  // Draw all the creatures
  for(i = creatureList.begin(); i != creatureList.end(); ++i){
    retVal = (*i)->drawCreature(mainWindow);
    fprintf(log, "drawing creature %i\n", retVal);// DEBUG
  }
  refresh();
  // Waiting for input
  getch();

}
