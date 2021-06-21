#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <SDL2/SDL.h>
#include "../Game.h"
#include "../EntityManager.h"
#include "./TransformComponent.h"

/*
String collider tag, int x, int xDelta, int y, int yDelta, int width, int deltaW, int height, int deltaH
*/
class ColliderComponent : public Component
{
public:
    std::string colliderTag;
    SDL_Rect collider;
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;
    TransformComponent *transform;

    int deltaX = 0;
    int deltaY = 0;
    int deltaW = 0;
    int deltaH = 0;
    bool isDebugging;

    //Provide extra values to modify the collision box
    ColliderComponent(std::string colliderTag, int x, int deltaX, int y, int deltaY, int width, int deltaW, int height, int deltaH)
    {
        this->colliderTag = colliderTag;

        //Initialize a new SDL Rectangle
        this->collider = {x, y, width, height};

        this->deltaX = deltaX;
        this->deltaY = deltaY;
        this->deltaW = deltaW;
        this->deltaH = deltaH;
    }

    ColliderComponent(std::string colliderTag, int x, int y, int width, int height)
    {
        this->colliderTag = colliderTag;

        //Initialize a new SDL Rectangle
        this->collider = {x, y, width, height};
    }

    void Initialize() override
    {
        if (owner->HasComponent<TransformComponent>())
        {
            transform = owner->GetComponent<TransformComponent>();

            //The source rectangle is the same size as the entity's transform
            sourceRectangle = {0, 0, transform->width, transform->height};

            //The destination rectangle is this component's collider
            destinationRectangle = {collider.x, collider.y, collider.w, collider.h};
        }
    }

    void Update(float deltaTime) override
    {
        //The collider follows the entity's transform and camera movement
        collider.x = static_cast<int>(transform->position.x) - Game::camera.x;
        collider.y = static_cast<int>(transform->position.y) - Game::camera.y;

        //The collider width and height also follows the transform
        collider.w = static_cast<int>(transform->width) * transform->scale;
        collider.h = static_cast<int>(transform->height) * transform->scale;

        //The collider's position and size can be adjusted
        collider.x += deltaX;
        collider.y += deltaY;
        collider.w += deltaW;
        collider.h += deltaH;

        destinationRectangle.x = collider.x - Game::camera.x;
        destinationRectangle.y = collider.y - Game::camera.y;

        if (Game::event.type == SDL_KEYDOWN)
        {
            if (Game::event.key.keysym.sym == SDLK_F1)
            {
                if (!isDebugging)
                {
                    isDebugging = true;
                }
                else
                    isDebugging = false;
            }
        }
    }

    void Render() override
    {
        /*if (isDebugging)
        {
            SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 0);
            SDL_RenderDrawRect(Game::renderer, &collider);
        }*/
    }
};

#endif

/*
What are the source and destination rectangles used for?

*/