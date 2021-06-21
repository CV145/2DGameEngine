#include <fstream>
#include "./Game.h"
#include "./Map.h"
#include "./EntityManager.h"
#include "./Components/TileComponent.h"

//Notice how manager is not set to anything in this file
//The "extern" keyword says that "manager" is defined in another file (Game.cpp)
extern EntityManager manager;

Map::Map(std::string textureId, int scale, int tileSize)
{
    this->textureId = textureId;
    this->scale = scale;
    this->tileSize = tileSize;
}

//Reads and loads from the map file
void Map::LoadMap(std::string filePath, int mapSizeX, int mapSizeY)
{
    std::ifstream mapFile;
    mapFile.open(filePath);

    if (!mapFile.is_open())
    {
        std::cerr << "ERROR" << std::endl;
        std::cerr << filePath << " was not able to be opened" << std::endl;
        exit(1);
    }

    /*
    The entire tile sprite sheet is stored as a texture in a map file. A source rectangle is used to go through each tile. The tiles are indexed like this:

    tile index = (row, column) and 0 = starting value
    */

    //For each row (y), go through each column (x), to get each tile index = yx
    for (int y = 0; y < mapSizeY; y++)
    {
        for (int x = 0; x < mapSizeX; x++)
        {
            //Retrieve the next character from the map file
            char ch;
            mapFile.get(ch);

            //atoi = ascii to integer
            int sourceRectY = atoi(&ch) * tileSize;

            mapFile.get(ch);
            int sourceRectX = atoi(&ch) * tileSize;

            AddTile(sourceRectX, sourceRectY, x * (scale * tileSize), y * (scale * tileSize));

            //Ignore the commas in the map file
            mapFile.ignore();
        }
    }

    mapFile.close();
}

//Adds a new tile to the map scene by passing in the location of the source rectangle (tile map iterator) and the actual location of the tile in the map
void Map::AddTile(int sourceRectX, int sourceRectY, int x, int y)
{
    Entity &newTile(manager.AddEntity("Tile", TILEMAP_LAYER));
    newTile.AddComponent<TileComponent>(sourceRectX, sourceRectY, x, y, tileSize, scale, textureId);
}