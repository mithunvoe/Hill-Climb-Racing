#include "Component.hpp"
#include "../Vector2D.hpp"
class TransformComponent : public Component
{
public:
    Vector2D position = Vector2D();
    Vector2D velocity = Vector2D();
    float height = 32, width = 32, scale = 1;
    float time = 1; // how fast velocity applies
    TransformComponent() {}
    TransformComponent(float x, float y)
    {
        position.x = x;
        position.y = y;
    }
    TransformComponent(float x, float y, float w, float h, float sc)
    {
        position.x = x;
        position.y = y;
        height = h;
        width = w;
        scale = sc;
    }
    void update() override
    {
        position.x += velocity.x * time;
        position.y += velocity.y * time;
    }
};