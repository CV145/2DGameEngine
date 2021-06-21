#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "../EntityManager.h"
#include "glm/glm.hpp"
#include <SDL2/SDL.h>
#include "../Game.h"
#include "./Component.h"

//PosX, PosY, VelX, VelY, Width, Height, Scale
class TransformComponent : public Component
{
public:
    glm::vec2 position;
    glm::vec2 velocity;
    int width;
    int height;
    int scale;
    bool isWaiting = false;
    float currentTime = 0;

    TransformComponent(int posX, int posY, int velX, int velY, int w, int h, int s)
    {
        position = glm::vec2(posX, posY);
        velocity = glm::vec2(velX, velY);
        width = w;
        height = h;
        scale = s;
    }

    void Initialize() override
    {
    }

    void UpdateVelocity(int velX, int velY)
    {
        velocity = glm::vec2(velX, velY);
    }

    void Update(float deltaTime) override
    {
        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;
    }

    void Render() override
    {
    }
};

#endif