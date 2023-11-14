#include "Map.hpp"
#include "Game.hpp"
#include <fstream>
#include "TextureManager.hpp"
Map::Map()
{
}
Map::~Map()
{
}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
    // SDL_Texture *tileset = TextureManager::loadTexture("assets/tile.png");
    // for (int y = 0; y < sizeY; y++)
    // {
    //     for (int x = 0; x < sizeX; x++)
    //     {
    //         SDL_Rect src, dest;
    //         src.w = src.h = dest.w = dest.h = 10;
    //         src.y = 1;
    //         if (y < 50)
    //         {
    //             src.x = 1;
    //         }
    //         else
    //         {
    //             src.x = 10;
    //         }
    //         dest.x = x;
    //         dest.y = y;
    //         TextureManager::Draw(tileset, src, dest);
    //     }
    // }

    char tile;
    std::fstream mapFile;
    mapFile.open(path);

    for (int y = 0; y < sizeY; y++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            mapFile.get(tile);
            tile -= '0';
            if (tile < 0 or tile > 2)
                tile = 0;
            Game::AddTile(tile, x * 64, y * 64, 64, 64);
            mapFile.ignore();
        }
    }

    mapFile.close();
}