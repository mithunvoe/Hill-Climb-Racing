#include "TextureManager.hpp"
#include "GameObject.hpp"
GameObject::GameObject(const char *texturesheet, int x, int y)
{
    objTexture = TextureManager::loadTexture(texturesheet);
    xpos = x;
    ypos = y;
}

int GameObject::getX() { return destRect.x; }
int GameObject::getHeight() { return destRect.h; }
int GameObject::getY() { return destRect.y; }
int GameObject::getWidth() { return destRect.w; }

void GameObject::update()
{
    xpos++;
    srcRect.h = 215;
    srcRect.w = 325;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = srcRect.w / 5;
    destRect.h = srcRect.h / 5;
}
void GameObject::render()
{
    SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}
