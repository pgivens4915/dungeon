#include <ncurses.h>
#include "creature.h"

int Creature::drawCreature(WINDOW* window){
  mvaddch(x, y, blit);
}

Creature::Creature(int inX, int inY, char inBlit){
  x = inX;
  y = inY;
  blit = inBlit;
}

