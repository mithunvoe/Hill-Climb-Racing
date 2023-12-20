#include "SDL2/SDL.h"
#include "Component.hpp"
#include "../TextureManager.hpp"
#include "Animation.hpp"
#include <map>

class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    TransformComponent *bgTransform;
    SDL_Texture *texture;
    int animated = 0;
    int frames = 0;
    int speed = 100;

public:
    int animIndex = 0;
    SDL_Rect srcRect, destRect;
    std::map<const char *, Animation> animations;
    void setTex(const char *path)
    {
        texture = TextureManager::loadTexture(path);
    }
    SpriteComponent() = default;
    SpriteComponent(const char *path)
    {
        setTex(path);
    }
    SpriteComponent(const char *path, int isAnimated, int frames, const char *animName)
    {
        setTex(path);
        animated = isAnimated;
        Animation coin = Animation(0, frames, 50);
        animations.emplace(animName, coin);
        play(animName);
    }
    SpriteComponent(const char *path, int isAnimated, int frames, const char *animName, TransformComponent &hehe)
    {
        setTex(path);
        animated = isAnimated;
        Animation coin = Animation(0, frames, 50);
        animations.emplace(animName, coin);
        play(animName);
        bgTransform = &hehe;
    }
    SpriteComponent(SDL_Texture *tex)
    {
        texture = tex;
    }

    ~SpriteComponent() { SDL_DestroyTexture(texture); }
    void setTexfromTex(SDL_Texture *tex)
    {
        texture = tex;
    }
    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        srcRect.x = srcRect.y = destRect.x = destRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }
    void remove()
    {
        texture = NULL;
    }
    void update() override
    {
        // cout << animated << ". ";
        if (animated == 1)
        {
            srcRect.x = srcRect.w * static_cast<int>(((SDL_GetTicks() / speed) % frames));
        }
        if (animated == 2)
        {
            srcRect.x = srcRect.w * static_cast<int>((int)(transform->position.x * .5) % frames);
        }
        if (animated == 3)
        {
            srcRect.x = srcRect.w * static_cast<int>((((int)(-bgTransform->position.x * .5) % frames)+frames)%frames);
        }
        srcRect.y = animIndex * transform->height;
        destRect.x = (int)transform->position.x;
        destRect.y = (int)transform->position.y;
    }
    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect);
    }

    void play(const char *animName)
    {
        // cout << animations["Walk"].frames << endl;
        // cout << animName << endl;
        // cout << frames + 100 << endl;
        frames = animations[animName].frames;
        animIndex = animations[animName].index;
        speed = animations[animName].speed;
    }
};
