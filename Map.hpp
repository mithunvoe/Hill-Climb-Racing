#pragma once
#include<string>

class Map
{
public:
    static void LoadMap(std::string path, int sizeX, int sizeY);
    Map();
    ~Map();

private:
};