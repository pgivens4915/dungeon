#ifndef MAP
#define MAP

class Map{
    public:
      // Intilizes the map object from a file
      int initMap();
      // Draws the visual content of the map  
      int drawMap(WINDOW* window);
    private:
      // The height and width of the map
      const static char width = 100;
      const static char height = 100;
      char map [height][width]; 
};

#endif

