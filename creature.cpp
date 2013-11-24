#include <ncurses.h>
#include "creature.h"
#include <cmath>

struct Tile{
  int x;
  int y;
  int cost;
  struct Tile* parent;
  // F is G + H
  int F;
  // G is the current path cost
  int G;
  // H is the estimated cost to target
  int H;
};

int estimate(int x, int y, int targetX, int targetY);

int Creature::drawCreature(WINDOW* window){
  mvaddch(y, x, blit);
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

int Creature::move(int TargetX,int TargetY){
    std::list<struct Tile> openList;
    std::list<struct Tile> closedList;
    struct Tile currentTile;
    currentTile.x = x;
    currentTile.y = y;
    currentTile.cost = 0;
    currentTile.H = estimate(x, y, TargetX, TargetY);
    currentTile.G = 0;
    currentTile.F = currentTile.H + currentTile.G;
    currentTIle.parent = NULL;
    
    openList.push_front(currentTile);

    // While we are not there or we have no options left
    while((currentTile.x != x && currentTile.y != y) || openList.empty()){
      currentTile = lowestCost(&openList);

    }
}

// Calcuating the estimate for a*
int estimate(int x, int y, int targetX, int targetY){
  // Int floor pythagareon theorum
  int deltaX = std::abs(x - targetX);
  int deltaY = std::abs(y - targetY);
  return ( (int)floor(sqrt( pow(deltaX, 2) + pow(deltaY, 2) )) );
}

// Returns the lowest cost tile in the open list
struct Tile* lowestCost(  
