
#ifndef kb_hpp
#define kb_hpp

#include "../Game.hpp"
#include "ECS.hpp"
#include "Component.hpp"

class KeyboardController : public Component
{
private:
public:
    TransformComponent *transform;
    SpriteComponent *sprite;
    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }
    void update() override
    {
        if (Game::event.type == SDL_KEYDOWN)
        {
            switch (Game::event.key.keysym.sym)
            {

            case SDLK_RIGHT:
                transform->velocity += Vector2D(-.2, 0);

                Game::currentFuel -= 0.25;
                // cout << Game::currentFuel << endl;

                break;
            case SDLK_LEFT:
                transform->velocity += Vector2D(.2, 0);
                Game::currentFuel -= 0.25;

                break;
            case SDLK_UP:
                transform->position.y -= 1;
                break;
            case SDLK_DOWN:
                transform->position.y += 1;
                break;
            case SDLK_SPACE:
                transform->velocity /= Vector2D(1.5, 1.5);
                break;

            default:
                break;
            }
        }
        else if (abs(transform->velocity.x) > 0.00001)
        {
            transform->velocity /= Vector2D(1.01, 1.01);
        }
    }

    KeyboardController(/* args */) = default;
    ~KeyboardController() = default;
};
#endif