#ifndef GameObject_hpp
#define GameObject_hpp
#include "Game.hpp"

class GameObject
{
public:
    GameObject(const char *texturesheet, int x, int y);
    ~GameObject();
    void update();
    void render();
    int getHeight();
    int getX();
    int getY();
    int getWidth();

    int xpos;
    int ypos;
    SDL_Texture *objTexture;
    SDL_Rect srcRect, destRect;
    
private:
};
#endif