#include <ncurses.h>
#include <math.h>
#include <unistd.h>
#include <unordered_map>
#include <cstdlib>
#include <float.h>
#include <cmath>
#include <climits>
#include "creature.h"

#define VISION_RANGE 7

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
void addNeighbors(struct Tile* currentTile, 
    std::unordered_map<int, Tile>* openList,
    std::unordered_map<int, Tile>* closedlist, Map* map, int targetX, 
    int targetY);
std::unordered_map<int, Tile>::iterator lowestCost(std::unordered_map<int, Tile>* openList);
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
  hunger = 300;
  hungry = false;
}

void hungerTick(Creature* creature){
  creature->hunger--;
  if (creature->hunger < 0){
    creature->dead = true;
  }
}

// Eats an item
void Creature::eat(Item* item, Map* map){
  hunger += 1000;
  map->remove(item);
   
                  
  hungry = false;
}

// Checks to see if we are on top of food, and if so, eat it
bool Creature::onFood(Map* map){
  std::list<Item*>::iterator it;
  
  for(it = map->itemList.begin(); it != map->itemList.end(); it++){
    // Are we on some food
    if(x == (*it)->x && y == (*it)->y && (*it)->blit == 'f'){
      // Eat the item
      // Pun not intended
      eat(*it, map);
      return true;
    }
  }
  return false;
}

// Sets a path to the closest (euclidian) food
void Creature::findFood(Map* map){
  // Declarations
  std::list<Item*>::iterator it;
  // The max possible distance, everything else is smaller
  double smallest = sqrt(pow(map->height, 2) * pow(map->width, 2));
  double dist;
  int targetX = 0;
  int targetY = 0;

  // Exit if we are on our way
  if (this->hungry == true) return;

  // Empty the current list
  this->emptyList();

  // Find the closest square
  for(it = map->itemList.begin(); it != map->itemList.end(); it++){
    dist = estimate(x,y, (*it)->x, (*it)->y);
    if (smallest > dist){
      smallest = dist;
      targetX = (*it)->x;
      targetY = (*it)->y;
    }
  }
  this->move(targetX, targetY, map);
  
  // Setting the hungry flag
  hungry = true;
}

// Recursive algorithm for sight
void Creature::shadowCast(Map* map, double startSlope, double endSlope,
                          int range, int startX, int startY){
  double newEndSlope;
  double newStartSlope;
  double jFinish = 0;
  double jStart = 0;
  int i = 0;
  int j = 0;

  for(i = range; i < VISION_RANGE; i++){
    // From the first seen slope to the last
    jStart = i / startSlope;
    jFinish = i / endSlope;
    for(j = round(jStart); j <= jFinish; j++){
      // If the view is not obscured
      if (map->map[startY - i][startX + j] != 'X'){
        // Do what needs to be done
        mvprintw(startY - i, startX + j, "%i", range);
      }
      else {
        // Calculate the angle in radians
        newEndSlope = (double)i/(double)(j - 1);
        mvprintw(20 + range, 0, "%f %i %i" , newEndSlope, j, i);
        shadowCast(map, startSlope, newEndSlope, i + 1, startX, startY);
        while(map->map[startY - i][startX + j] == 'X'){
          j++;
          if (j > jFinish) return;
          else{
            newStartSlope = (double)i/(double)j;
            shadowCast(map, newStartSlope, endSlope, i, startX, startY);
            return;
          }
        }
      }
    }
  }
}

// The creatures shadowcasting algorithm
void Creature::look(Map* map){
  // First octant
  shadowCast(map, -1, INFINITY, 0, x, y);
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


  // If we are hungry what do we do
  if(hunger < 300){
    // Funny syntax, eat the food if we are on it else walk to it
    if (onFood(map)){
    }

    else findFood(map);
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

  // Look around
  look(map);

  return(0);
}

int cartToInt(int x, int y){
  return(x * 1000 + y);
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
  std::unordered_map<int, Tile> openList;
  std::unordered_map<int, Tile> closedList;
  std::unordered_map<int, Tile>::iterator iterator; 
  // Becuase I dont want to malloc =)
  struct Tile aTile;
  struct Tile* currentTile = &aTile;
  // The key value
  int key = cartToInt(x,y);

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
  openList.insert({key,*currentTile});

  // While we have no options left
  while(!openList.empty()){
    // Get the position that we want to delete, it is the lowest cost
    // square
    iterator = lowestCost(&openList);
    // Delete that node from the open list
    openList.erase(iterator);
    // Put it on the closed list
    closedList.insert(*iterator);
    // It is your new current tile
    currentTile = &(iterator->second);
    // If we have reached are target exit
    if(currentTile->x == targetX && currentTile->y == targetY) break;
    // Adds the neighbors of the tile to the open list
    // or adjusts their parents
    addNeighbors(currentTile, &openList, &closedList, map, targetX, targetY);

  }
  // Returns the path to the movement stack
  if(!openList.empty()) returnPath(currentTile);
  return(0);
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
void addNeighbors(struct Tile* currentTile, 
    std::unordered_map<int, Tile>* openList,
    std::unordered_map<int, Tile>* closedList, Map* map, int targetX, 
    int targetY){

  // Declarations
  int key;
  struct Tile newTile;
  bool onOpenList;
  bool onClosedList;
  std::unordered_map<int, Tile>::iterator it;

  // For all adjacent tiles
  for(int i = -1; i < 2; i++){
    for(int j = -1; j < 2; j++){
      // Checking to see if they are on the lists
      // Set flags appropriatly, if nothing exists, the flag should
      // be zero
      key = cartToInt(currentTile->x + j, currentTile->y + i);
      onOpenList = openList->count(key); 
      onClosedList = closedList->count(key);

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
        key = cartToInt(newTile.x, newTile.y);
        openList->insert({key, newTile});
      }

      // Else if it is on the open list make sure we have the shortest cost 
      else if(!(i == 0 && j == 0) && 
          map->map[currentTile->y + i][currentTile->x + j] != 'X' && 
          !onClosedList){

        //Find the right item
        key = cartToInt(currentTile->x + j, currentTile->y + i);
        it = openList->find(key);
        // If we have found a shorter path
        // TODO: Better metrics here
        if(it->second.G > currentTile->G + 1){
          it->second.G = currentTile->G + 1;
          // Setting the parent
          it->second.parent = currentTile; 
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
std::unordered_map<int,Tile>::iterator lowestCost(
    std::unordered_map<int, Tile>* openList){

  //Declarations
  std::unordered_map<int, Tile>::iterator it; 
  std::unordered_map<int, Tile>::iterator iteratorPosition; 

  // Initiate the largest tile, in this case zero
  struct Tile smallestMalloc;
  struct Tile* smallest = &smallestMalloc;
  // Make its value the max so everything will be smaller
  smallest->F = DBL_MAX;

  // For every item on the list
  for(it = openList->begin(); it != openList->end(); it++){
    // If it is smaller
    if (it->second.F < smallest->F){
      // Pointer assignment, smallest should point to a member of the 
      // list that was passed in
      smallest = &(it->second);
      iteratorPosition = it;
    }
  }
  return(iteratorPosition);
}

// Reseting the path
void Creature::emptyList(){
  path = std::stack<int>();
}
