#ifndef kb_hpp
#define kb_hpp

#include "../Game.hpp"
#include "ECS.hpp"
#include "Component.hpp"

class KeyboardController : public Component
{
private:
public:
    TransformComponent *transform, *bgtrans;
    SpriteComponent *sprite;
    double angle, ground;
    double matir_y;
    double jiniser_y;
    KeyboardController(TransformComponent &hehe)
    {
        bgtrans = &hehe;
    }
    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
        ground = transform->position.y;
    }
    void update() override
    {
        if (!Game::inMenu)
        {
            double v = .5 / 10, g = .01 / 3;
            double x = bgtrans->position.x + 480;
            matir_y = Game::dq[Game::i - 1] ? 2 * 1000000 / (x * x + 10000) : 8.319;
            jiniser_y = transform->position.y;
            if (entity->getComponent<SpriteComponent>().entityName == (string) "coin")
            {
                matir_y += ground;
                ;
                transform->velocity.x = bgtrans->velocity.x;
            }
            angle = Game::dq[Game::i - 1] ? atan(-2 * (2000000 * x) / ((x * x + 10000) * (x * x + 10000))) : 0;

            if (jiniser_y > matir_y + 1)
            {
                transform->velocity.y -= g;
            }
            else if (jiniser_y < matir_y - 1)
            {
                if (transform->velocity.y < 0)
                    transform->velocity.y = 0;
                transform->position.y = matir_y;
            }

            transform->velocity -= Vector2D(g * sin(angle) * cos(angle), g * sin(angle) * sin(angle));

            // if (Game::event.type==SDL_MOUSEBUTTONDOWN)
            //     if (!Game::inMenu && Game::event.button.button == SDL_BUTTON_LEFT)
            //     {

            //     }

            if (Game::event.type == SDL_KEYDOWN || Game::moveRight || Game::moveLeft)
            {
                double rightFuelLoss = 1, leftFuelLoss = .75, factor = 15, rotateAngle = 15;
                if (Game::event.key.keysym.sym == SDLK_RIGHT || Game::moveRight)
                {
                    // cout<<Game::moveLeft<<endl;
                    if (Game::moveRight)
                    {
                        v /= factor;
                        rightFuelLoss /= factor;
                        rotateAngle /= factor;
                    }
                    if (abs(jiniser_y - matir_y) > 5)
                    {
                        entity->manager.getGroup(1)[0]->getComponent<SpriteComponent>().torque = -rotateAngle;
                    }
                    else
                    {
                        transform->velocity -= Vector2D(v * cos(angle) * cos(angle), v * cos(angle) * sin(angle));
                    }
                    if (entity->getComponent<SpriteComponent>().entityName == "amibg")
                        Game::currentFuel -= rightFuelLoss;
                    if (Game::moveRight)
                    {
                        v *= factor;
                        rightFuelLoss *= factor;
                        rotateAngle *= factor;

                    } // Game::moveRight=0;
                }
                else if (Game::event.key.keysym.sym == SDLK_LEFT || Game::moveLeft)
                {
                    if (Game::moveLeft)
                    {
                        v /= factor;
                        leftFuelLoss /= factor;
                        rotateAngle /= factor;
                    }
                    if (abs(jiniser_y - matir_y) > 5)
                        entity->manager.getGroup(1)[0]->getComponent<SpriteComponent>().torque = rotateAngle;
                    else
                    {
                        if (transform->velocity.x > 1)
                            transform->velocity /= Vector2D(1.5, 1.5);
                        else
                            transform->velocity += Vector2D(.5 * v * cos(angle) * cos(angle), .5 * v * cos(angle) * sin(angle));
                    }

                    if (entity->getComponent<SpriteComponent>().entityName == "amibg")
                        Game::currentFuel -= leftFuelLoss;
                    // Game::moveLeft=0;
                    if (Game::moveLeft)
                    {
                        v *= factor;
                        leftFuelLoss *= factor;
                        rotateAngle *= factor;
                    }
                }
            }

            else if (abs(jiniser_y - matir_y) < 1)
            {
                transform->velocity /= Vector2D(1.001, 1.001);
            }
            else
                transform->velocity /= Vector2D(1.0001, 1.0001);
        }
    }
    KeyboardController(/* args */) = default;
    ~KeyboardController() = default;
};

#endif