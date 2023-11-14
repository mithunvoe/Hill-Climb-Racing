#pragma once
#include "ECS.hpp"
#include "SDL2/SDL.h"
#include "Component.hpp"

class TileComponent : public Component
{
public:
    TransformComponent *transform;
    SpriteComponent *sprite;
    SDL_Rect tileRect;
    int tileID;
    char *path;
    
    TileComponent() = default;
    TileComponent(int x, int y, int w, int h, int id)
    {
        tileRect.x = x;
        tileRect.y = y;
        tileRect.h = h;
        tileRect.w = w;
        tileID = id;
        // switch (tileID)
        // {
        // case 0:
        //     path = "assets/water.png";
        //     break;
        // case 1:
        //     path = "assets/dirt.png";
        //     break;
        // case 2:
        //     path = "assets/grass.png";
        //     break;
        // default:
        //     break;
        // }
    }
    void init() override
    {

        entity->addComponent<TransformComponent>((float)tileRect.x, (float)tileRect.y, (float)tileRect.h, (float)tileRect.w, 1);
        transform = &entity->getComponent<TransformComponent>();

        entity->addComponent<SpriteComponent>(path);
        sprite = &entity->getComponent<SpriteComponent>();
    }
};
