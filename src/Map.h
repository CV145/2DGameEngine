#ifndef MAP_H
#define MAP_H

#include <string>

class Map
{
private:
    std::string textureId;
    int scale;
    int tileSize;

public:
    Map(std::string textureId, int scale, int tileSize);
    ~Map();
    void LoadMap(std::string filePath, int mapSizeX, int mapSizeY);
    void LoadMap();
    void AddTile(int sourceX, int sourceY, int x, int y);
};

#endif

/*
If the map is scaled by 2 you're doing a 2x zoom in
*/