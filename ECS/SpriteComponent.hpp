#include "SDL2/SDL.h"
#include "Component.hpp"
#include "../TextureManager.hpp"
#include "Animation.hpp"
#include <map>

class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    TransformComponent *bgTransform = nullptr;
    int animated = 0;
    int frames = 0;
    int speed = 100;

public:
    SDL_Texture *texture;
    int animIndex = 0;
    string entityName;
    SDL_Rect srcRect, destRect;
    float angle = 0, prevAngle = 0;
    double x;
    double matir_y, jiniser_y;
    double torque = 0;
    int isGameOver = 0;
    std::map<const char *, Animation> animations;
    SpriteComponent() = default;

    void setTexfromTex(SDL_Texture *tex)
    {
        texture = tex;
    }

    SpriteComponent(const char *path, const char *animName)
    {
        texture = TextureManager::loadTexture(path);
        entityName = animName;
    }
    SpriteComponent(const char *path)
    {
        texture = TextureManager::loadTexture(path);
    }
    SpriteComponent(const char *path, int isAnimated, int frames, const char *animName)
    {
        texture = TextureManager::loadTexture(path);
        entityName = animName;
        animated = isAnimated;
        Animation coin = Animation(0, frames, 50);
        animations.emplace(animName, coin);
        play(animName);
    }
    SpriteComponent(SDL_Texture *tex, int isAnimated, int frames, const char *animName)
    {
        texture = tex;
        entityName = animName;
        animated = isAnimated;
        Animation coin = Animation(0, frames, 50);
        animations.emplace(animName, coin);
        play(animName);
    }
    SpriteComponent(const char *path, int isAnimated, int frames, const char *animName, TransformComponent &hehe)
    {
        texture = TextureManager::loadTexture(path);
        entityName = animName;
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
    SpriteComponent(SDL_Texture *tex, const char *animName)
    {
        texture = tex;
        entityName = animName;
    }

    ~SpriteComponent() { SDL_DestroyTexture(texture); }
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
        if (bgTransform != nullptr)
        {
            x = bgTransform->position.x + 480;
            if (Game::dq[Game::i - 1])
                matir_y = 2 * 1000000 / (x * x + 10000);
            else
                matir_y = 8.319;
            jiniser_y = bgTransform->position.y;
            prevAngle = angle;
            prevAngle += torque;
            torque = 0;
            x += 32.5;
            angle = Game::dq[Game::i - 1] ? atan(-2 * (2000000 * x) / ((x * x + 10000) * (x * x + 10000))) : 0;
            angle *= 180 / 3.1416;
        }
        if (animated == 1)
            srcRect.x = srcRect.w * static_cast<int>(((SDL_GetTicks() / speed) % frames));
        if (animated == 2)
            srcRect.x = srcRect.w * static_cast<int>((int)(transform->position.x * .5) % frames);
        if (animated == 3)
            srcRect.x = srcRect.w * static_cast<int>((((int)(-bgTransform->position.x * .5) % frames) + frames) % frames);

        srcRect.y = animIndex * transform->height;
        destRect.x = (int)transform->position.x;
        destRect.y = (int)transform->position.y;
    }
    void draw() override
    {
        if (animated == 3)
        {
            auto angle2 = -angle;
            auto prevAngle2 = -prevAngle;
            // cout << "gari: " << jiniser_y << "matir y: " << matir_y << endl;
            // cout << "garir angle: " << prevAngle2 << "matir angle: " << angle2 << endl;
            if (abs(jiniser_y - matir_y) > 2)
                angle = prevAngle;
            else
            {
                if (prevAngle2 - angle2 > 90 and prevAngle2 - angle2 < 270)
                    angle = prevAngle + (angle - 179 - prevAngle) / 20;
                else
                    angle = prevAngle + (angle - prevAngle) / 20;
            }
            if (prevAngle2 - angle2 > 170 and prevAngle2 - angle2 < 190 and abs(jiniser_y - matir_y) < .01 and isGameOver < 0)
                isGameOver = 600;
            TextureManager::DrawGari(texture, srcRect, destRect, angle);
        }
        else
            TextureManager::Draw(texture, srcRect, destRect);
    }

    void play(const char *animName)
    {
        frames = animations[animName].frames;
        animIndex = animations[animName].index;
        speed = animations[animName].speed;
    }
};
