#include <ncurses.h>
#include "creature.h"

int Creature::drawCreature(WINDOW* window){
  mvaddch(x, y, blit);
  return(0);
}

Creature::Creature(int inX, int inY, char inBlit){
  x = inX;
  y = inY;
  blit = inBlit;
  path.push(1001); 
  path.push(1002);
  path.push(1003);
  path.push(2003);
  path.push(2004);
  path.push(2005);
}

int Creature::step(){
  int ret = 0;

  // Return if the queue is empty
  if(path.empty()){
    return(-1);
  }

  // Getting (x,y) from the queue
  ret = path.front();
  path.pop();
  // The top part of ret is x and the bottom is y
  // For example 120021 x = 120 y = 21
  x = ret / 1000;
  y = ret % 1000;
  return(0);
}

