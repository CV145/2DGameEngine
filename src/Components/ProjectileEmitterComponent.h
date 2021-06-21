#ifndef PROJECTILEEMITTERCOMPONENT_H
#define PROJECTILEEMITTERCOMPONENT_H

#include <glm/gtx/vector_angle.hpp>
#include "glm/glm.hpp"
#include "../EntityManager.h"
#include "TransformComponent.h"

//int speed, int angleDeg, int range, bool shouldLoop - Note: angles are clockwise
class ProjectileEmitterComponent : public Component
{
private:
    TransformComponent *transform;
    glm::vec2 origin;
    glm::vec2 target;
    int speed;
    int range;
    float angleRad;
    Entity *parent; //parent is enemy entity
    EntityManager *manager;

public:
    bool isActive;
    bool isStored = true;
    bool atOrigin = false;
    bool targetAcquired = false;
    int shouldLoop;

    ProjectileEmitterComponent(int speed, int range, int shouldLoop, Entity &parent, EntityManager &manager)
    {
        this->speed = speed;
        this->range = range;
        this->shouldLoop = shouldLoop;
        this->parent = &parent;
        this->manager = &manager;
    }

    void Initialize() override
    {
        transform = owner->GetComponent<TransformComponent>();
    }

    void Update(float deltaTime) override
    {
        UpdateOrigin();

        if (!targetAcquired)
        {
            UpdateTarget();
            parent->GetComponent<SpriteComponent>()->isShooting = true;
        }

        if (isStored)
        {
            transform->position.x = -500;
            transform->position.y = -500;
        }

        if (isActive)
        {
            isStored = false;
            float xComp = target.x - origin.x;
            float yComp = target.y - origin.y;

            angleRad = glm::atan(yComp, xComp);
            UpdateVelocity();

            if (glm::distance(transform->position, origin) > range)
            {
                targetAcquired = false;

                if (shouldLoop)
                {
                    transform->position.x = origin.x;
                    transform->position.y = origin.y;
                }
                else
                {
                    isStored = true;
                }
            }
        }
        else
        {
            parent->GetComponent<SpriteComponent>()->isShooting = false;
        }
    }

    //Target is the player's (X,Y)
    void UpdateTarget()
    {
        target = glm::vec2(
            manager->GetPlayerAddress().GetComponent<TransformComponent>()->position.x,
            manager->GetPlayerAddress().GetComponent<TransformComponent>()->position.y);

        targetAcquired = true;
    }

    //Origin is center of wherever the enemy is
    void UpdateOrigin()
    {
        TransformComponent *parentTransform = parent->GetComponent<TransformComponent>();
        origin = glm::vec2(parentTransform->position.x + parentTransform->width / 2, parentTransform->position.y + parentTransform->height / 2);
    }

    //Velocity will cause the projectile to move
    void UpdateVelocity()
    {
        //Velocity is a vector made of the x and y components - update takes into account change in angle
        transform->velocity = glm::vec2(glm::cos(angleRad) * speed, glm::sin(angleRad) * speed);
    }
};

#endif