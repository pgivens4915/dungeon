#include <ncurses.h>
#include <cstdlib>
#include <float.h>
#include <cmath>
#include <climits>
#include "creature.h"

FILE* logg;

// A structure to hold the tile information for the A* Alg
struct Tile{
  int x;
  int y;
  double cost;
  struct Tile* parent;
  // F is G + H
  double F;
  // G is the current path cost
  double G;
  // H is the estimated cost to target
  double H;
};

// Function Declarations //////////////////////////////////////////////////////
double estimate(int x, int y, int targetX, int targetY);
void returnPath(struct Tile* currentTile);
void addNeighbors(struct Tile* currentTile, std::list<struct Tile>* openList,
    std::list<struct Tile>* closedlist, Map* map, int targetX, 
    int targetY);
std::list<struct Tile>::iterator lowestCost(std::list<struct Tile>* openList);
///////////////////////////////////////////////////////////////////////////////

Creature::~Creature(){
  // How you free a stack?
  path = std::stack<int>();
}

int Creature::drawCreature(WINDOW* window){
  mvaddch(y, x, blit);
  return(0);
}

// The constructor, more to come here
Creature::Creature(int inX, int inY, char inBlit){
  x = inX;
  y = inY;
  blit = inBlit;
  dead = false;
  hunger= 100;
}

void hungerTick(Creature* creature){
  creature->hunger--;
  if (creature->hunger < 0){
    creature->dead = true;
  }
}

// Everything that is needed to move a creature one step 
// Patfinding happens here if it is needed
int Creature::step(Map* map){
  int ret = 0;
  int randX;
  int randY;

  //Checking map
  if (map == NULL){
    return(0);
  }

  // If we are not moving what should we do? //////////////////////////////////
  // TODO
  if(path.empty()){
    do{
      randX = rand() % map->width;
      randY = rand() % map->height;
    }while(map->map[randY][randX] == 'X');
    this->move(randX, randY, map);
  }
  /////////////////////////////////////////////////////////////////////////////

  // Right now the only thing we really do is move
  // Getting (x,y) from the stack
  ret = path.top();
  path.pop();
  // The top part of ret is x and the bottom is y
  // For example 120021 x = 120 y = 21
  x = ret / 1000;
  y = ret % 1000;

   
  hungerTick(this);

  // DEBUG
  mvprintw(40,0,"HERE %i", hunger);
  refresh();
  // End DEBUG
  return(0);
}

// Prints some lists for debuging reasons
//void printLists(FILE* logg, std::list<struct Tile>* openList,
//                std::list<struct Tile>* closedList){
//  std::list<struct Tile>::iterator it;
//  fprintf(logg, "OPEN LIST\n");
//  for(it = openList->begin(); it != openList->end(); it++){
//    fprintf(logg, "%i,%i : %f %f %f\n", it->x, it->y, it->F, it->G, it->H);
//  }
//  fprintf(logg, "CLOSED LIST\n");
//  for(it = closedList->begin(); it != closedList->end(); it++){
//    fprintf(logg, "%i,%i : %f %f %f\n", it->x, it->y, it->F, it->G, it->H);
//  }
//}

// This function is our A* algorithm
int Creature::move(int targetX, int targetY, Map* map){
  std::list<struct Tile> openList;
  std::list<struct Tile> closedList;
  std::list<struct Tile>::iterator iterator; 
  // Becuase I dont want to malloc =)
  struct Tile aTile;
  struct Tile* currentTile = &aTile;

  // Initilize the starting tile
  currentTile->x = x;
  currentTile->y = y;
  currentTile->cost = 0;
  currentTile->H = estimate(x, y, targetX, targetY);
  currentTile->G = 0;
  currentTile->F = currentTile->H + currentTile->G;
  currentTile->parent = NULL;

  // If we are already there return
  if(currentTile->x == targetX && currentTile->y == targetY){
    return(0);
  }

  // Add the starting tile to the open list
  openList.push_front(*currentTile);

  // While we have no options left
  while(!openList.empty()){
    // Get the position that we want to delete, it is the lowest cost
    // square
    iterator = lowestCost(&openList);
    // Delete that node from the open list
    openList.erase(iterator);
    // Put it on the closed list
    closedList.push_front(*iterator);
    // It is your new current tile
    currentTile = &(*iterator);
    // If we have reached are target exit
    if(currentTile->x == targetX && currentTile->y == targetY) break;
    // Adds the neighbors of the tile to the open list
    // or adjusts their parents
    addNeighbors(currentTile, &openList, &closedList, map, targetX, targetY);

  }
  // Returns the path to the movement stack
  if(!openList.empty()) returnPath(currentTile);
}

// Returns the path constructed from A* and adds it to the creatures movement
// stack
void Creature::returnPath(struct Tile* currentTile){
  // Declarations
  int stepX;
  int stepY;
  int combo;

  // While we have not found the end
  while(currentTile->parent != NULL){
    stepX = currentTile->x;
    stepY = currentTile->y;
    // The path data structure consists of a int split so xxxyyy
    combo = (stepX * 1000) + stepY;
    path.push(combo);
    // Move to the next tile
    currentTile = currentTile->parent;
  }
}

// Check a list to see if it contains a tile object at x,y
bool contains(std::list<struct Tile>* list, int x, int y){
  std::list<struct Tile>::iterator it;
  for(it = list->begin(); it != list->end(); it++){
    if((*it).x == x && (*it).y == y){
      return true;
    }
  }
  return false;
}

// Checks all adjacent neighbors,  if a move is valid and they are not on the 
// open list it moves them onto the open list.  Else if it is valid and not on
// the closed list, adjust the parent if the path is shorter
void addNeighbors(struct Tile* currentTile, std::list<struct Tile>* openList,
    std::list<struct Tile>* closedList, Map* map, int targetX, 
    int targetY){

  // Declarations
  struct Tile newTile;
  bool onOpenList;
  bool onClosedList;
  std::list<struct Tile>::iterator it;

  // For all adjacent tiles
  for(int i = -1; i < 2; i++){
    for(int j = -1; j < 2; j++){
      // Checking to see if they are on the lists
      // Set flags appropriatly
      onOpenList = contains(openList, currentTile->x + j, 
          currentTile->y + i);
      onClosedList = contains(closedList, currentTile->x + j, 
          currentTile->y + i);

      // If it is not the current tile, passalbe, and not on the list
      // That means the node is new
      // We add it to the open list if that was the case
      if(!(i == 0 && j == 0) && 
          map->map[currentTile->y + i][currentTile->x + j] != 'X' && 
          !onClosedList && !onOpenList){

        // Create a new tile that represents the adjacent tile
        newTile.x = currentTile->x + j;
        newTile.y = currentTile->y + i;
        newTile.cost = 1;
        // The diagonal case
        if(i != 0 && j != 0) newTile.cost = 1.4;
        newTile.G = newTile.cost + currentTile->G;
        newTile.H = estimate(newTile.x, newTile.y, targetX, targetY);
        newTile.F = newTile.G + newTile.H;
        newTile.parent = currentTile;

        // Add that new tile to the open list
        openList->push_front(newTile);
      }

      // Else if it is on the open list make sure we have the shortest cost 
      else if(!(i == 0 && j == 0) && 
          map->map[currentTile->y + i][currentTile->x + j] != 'X' && 
          !onClosedList){

        // Find the right item
        for(it = openList->begin(); it != openList->end(); it++){
          // If it matches the x y coord
          if(it->x == currentTile->x + j && it->y == currentTile->y + i){
            // If we have found a shorter path
            // TODO: Better metrics here
            if(it->G > currentTile->G + 1){
              it->G = currentTile->G + 1;
              // Setting the parent
              it->parent = currentTile; 
            }
          }
        }
      }
    }
  }
}

// Calcuating the estimate for a*
double estimate(int x, int y, int targetX, int targetY){
  // Int floor pythagareon theorum
  int deltaX = std::abs(x - targetX);
  int deltaY = std::abs(y - targetY);
  return ( (sqrt( pow(deltaX, 2) + pow(deltaY, 2) )) );
}

// Returns the lowest cost tile in the open list
std::list<struct Tile>::iterator lowestCost(std::list<struct Tile>* openList){
  std::list<struct Tile>::iterator it; 
  std::list<struct Tile>::iterator iteratorPosition; 

  // Initiate the largest tile, in this case zero
  struct Tile smallestMalloc;
  struct Tile* smallest = &smallestMalloc;
  // Make its value the max so everything will be smaller
  smallest->F = DBL_MAX;

  // For every item on the list
  for(it = openList->begin(); it != openList->end(); it++){
    // If it is smaller
    if (it->F < smallest->F){
      // Pointer assignment, smallest should point to a member of the 
      // list that was passed in
      smallest = &(*it);
      iteratorPosition = it;
    }
  }
  return(iteratorPosition);
}
