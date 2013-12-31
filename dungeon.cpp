#include <ncurses.h>
#include <sys/resource.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include "map.h"
#include "creature.h"
#include <list>

#define MAX_FPS 3


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
  map.addCreature(1, 1, 'r');
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
  const double second = 1000000;
  std::list<Creature*>::iterator i;
  int retVal;
  int microSeconds;
  char c;
  struct rusage timeQuery;
  struct timeval startTime;
  struct timeval endTime;

  // Start a timer
  getrusage(RUSAGE_SELF, &timeQuery);
  startTime = timeQuery.ru_stime;

  // Draw everything
  map.drawMap(mainWindow);
  // Draw all the creatures
  map.drawMoveCreatures(mainWindow);
  refresh();

  // Waiting for input
  //do {
  //    c = getch();
  //} while (c != '.');

  // Waiting for the correct amount of time
  getrusage(RUSAGE_SELF, &timeQuery);
  endTime = timeQuery.ru_stime;

  // Calculating the time in microseconds
  microSeconds = (startTime.tv_sec - endTime.tv_sec) * second +
                  (startTime.tv_usec - endTime.tv_usec);
  // Sleep for the correct time 
  if (microSeconds < (second / MAX_FPS)){
    usleep((second / MAX_FPS) - microSeconds);
  }
}
