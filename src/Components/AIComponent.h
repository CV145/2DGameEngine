#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#define ROAMING_STATE 1
#define ATTACKING_STATE 2
#define FLEEING_STATE 3

#include <vector>
#include <SDL2/SDL.h>
#include "glm/glm.hpp"
#include "../EntityManager.h"
#include "./Component.h"
#include "./ProjectileEmitterComponent.h"
#include "./TransformComponent.h"
#include "./ColliderComponent.h"
#include "../Collision.h"
#include "../Game.h"

class AIComponent : public Component
{
public:
    struct AI_State
    {
        int aiState = ROAMING_STATE;
    };

    AI_State self;

    EntityManager *manager;
    std::vector<Entity *> waypoints;
    Entity *projectile;
    ProjectileEmitterComponent *emitter;
    TransformComponent *transform;
    TransformComponent *player;
    glm::vec2 target;
    ColliderComponent *thisCollider;
    ColliderComponent *targetCollider;
    float angleRad;
    int speed;
    bool searchingForWaypoint;

    AIComponent(EntityManager *manager, int speed, Entity *projectile)
    {
        this->manager = manager;
        waypoints = manager->GetWaypoints();
        searchingForWaypoint = true;
        this->speed = speed;
        this->projectile = projectile;
    }

    void Initialize() override
    {
        transform = owner->GetComponent<TransformComponent>();
        player = manager->GetPlayerAddress().GetComponent<TransformComponent>();

        emitter = projectile->GetComponent<ProjectileEmitterComponent>();
    }

    void Update(float deltaTime) override
    {
        CheckState();
        thisCollider = owner->GetComponent<ColliderComponent>();
        switch (self.aiState)
        {
        case ROAMING_STATE:
            emitter->isActive = false;
            emitter->shouldLoop = false;
            if (searchingForWaypoint)
            {

                //Note: Division by zero is undefined
                int randomWaypoint = rand() % waypoints.size();

                target = waypoints[randomWaypoint]->GetComponent<TransformComponent>()->position;

                float xComp = target.x - transform->position.x;
                float yComp = target.y - transform->position.y;

                angleRad = glm::atan(yComp, xComp);

                //Set velocity to direction of waypoint
                transform->velocity =
                    glm::vec2(glm::cos(angleRad) * speed, glm::sin(angleRad) * speed);

                //Grab target collider
                targetCollider = waypoints[randomWaypoint]->GetComponent<ColliderComponent>();

                searchingForWaypoint = false;
            }
            //If collision with waypoint, searchingForWaypoint is true
            else if (Collision::CheckRectangleCollision(targetCollider->collider, thisCollider->collider))
            {
                searchingForWaypoint = true;
            }

            break;

        case FLEEING_STATE:
            break;

        case ATTACKING_STATE:

            //Stop moving only if playing shoot animation
            if (owner->GetComponent<SpriteComponent>()->isShooting)
            {
                transform->velocity =
                    glm::vec2(0, 0);
            }

            emitter->isActive = true;
            emitter->shouldLoop = true;
            searchingForWaypoint = true;
            break;
        }
    }

    void CheckState()
    {
        int attackX = player->position.x - (32 * 8);
        int attackY = player->position.y - (32 * 8);
        int attackW = 32 * 16;
        int attackH = 32 * 16;

        //std::cout << "Player X: " << player->position.x << " and Player Y: " << player->position.y << std::endl;
        //std::cout << "Attack X: " << attackX << " and Attack W: " << attackX + attackW << std::endl;
        //std::cout << "Attack Y: " << attackY << " and Attack H: " << attackY + attackH << std::endl;

        //Check if within certain distance from player to change states
        if (transform->position.x > attackX && transform->position.x < (attackX + attackW) && transform->position.y > attackY && transform->position.y < (attackY + attackH))
        {
            self.aiState = ATTACKING_STATE;
        }
        else
        {
            self.aiState = ROAMING_STATE;
        }
    }
};

#endif