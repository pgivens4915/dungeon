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
  int gameStep(WINDOW* mainWindow, Map map, bool paused);
  Map map;
  WINDOW *mainWindow;
  int retval;
  bool paused = true;

  // Map initilization
  retval = map.initMap();
  if(retval != 0){
    return(1);
  }
  
  map.addCreature(1, 1, 'r');
  log = fopen("log.txt", "w");

  // ncurses intit
  mainWindow = initscr();
  nodelay(mainWindow, TRUE);

  fprintf(log, "DEBUG\n");// DEBUG
  
  for(;;){
    paused = gameStep(mainWindow, map, paused);
  }
 
  endwin();

  return 0;
}

bool pausedLoop(){
  char c; 
  bool paused = true;
  do {
    c = getch();
    switch(c){ 
      case ' ' :
      paused = false;
      break;
    }
  } while (c != '.' && paused);
  return(paused);
}

// Takes a single step of the game
int gameStep(WINDOW* mainWindow, Map map, bool paused){
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

  if (paused){
  // Waiting for input
    paused = pausedLoop();
  }

  // else it is not paused
  else{
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

  // Getting unpaused input
  c = getch();
  if (c == ' '){
    paused = true;
  }

  return(paused);
}
