#include <iostream>
#include "./EntityManager.h"
#include "./Collision.h"
#include "./Components/ColliderComponent.h"

//Destroy all the entities
void EntityManager::ClearData()
{
    for (auto &entity : entities)
    {
        entity->Destroy();
    }
}

bool EntityManager::HasNoEntities()
{
    return entities.size() == 0;
}

void EntityManager::Update(float deltaTime)
{
    for (auto &entity : entities)
    {
        entity->Update(deltaTime);
    }

    DestroyInactiveEntities();
}

void EntityManager::DestroyInactiveEntities()
{
    for (int i = 0; i < entities.size(); i++)
    {
        if (!entities[i]->IsActive())
        {
            //Erase the entity at position i - shorten the vector by 1
            entities.erase(entities.begin() + i);
        }
    }
}

//Rendering order is determined by layers
void EntityManager::Render()
{
    for (int layerNumber = 0; layerNumber < NUM_LAYERS; layerNumber++)
    {
        for (auto &entity : GetEntitiesByLayer(static_cast<LayerType>(layerNumber)))
        {
            entity->Render();
        }
    }
}

Entity &EntityManager::AddEntity(std::string entityName, LayerType layer)
{
    Entity *entity = new Entity(*this, entityName, layer);
    entities.emplace_back(entity); //add entity to vector list

    if (layer == WAYPOINT_LAYER)
    {
        waypoints.emplace_back(entity);
    }

    return *entity; //return the entity that was added
}

Entity &EntityManager::GetPlayerAddress()
{
    std::vector<Entity *> entities = GetEntitiesByLayer(PLAYER_LAYER);
    return *entities[0];
}

std::vector<Entity *> EntityManager::GetEntities() const
{
    return entities;
}

std::vector<Entity *> EntityManager::GetWaypoints() const
{
    return waypoints;
}

std::vector<Entity *> EntityManager::GetEntitiesByLayer(LayerType layer) const
{
    std::vector<Entity *> selectedEntities;
    for (auto &entity : entities)
    {
        if (entity->layer == layer)
        {
            selectedEntities.emplace_back(entity);
        }
    }

    return selectedEntities;
}

unsigned int EntityManager::GetEntityCount()
{
    return entities.size();
}

//Lists all the entities in the game and the components they have in the terminal
void EntityManager::ListAllEntities()
{
    std::vector<Entity *> entities = GetEntities();

    for (Entity *entity : entities)
    {
        std::cout << "Entity Name: " << entity->name << std::endl;
        entity->ListAllComponents();
    }
}

//The given entity checks against all other entities for collisions. If there is a collision, return the tag of the other entity's collider
std::string EntityManager::CheckEntityCollisions(Entity &myEntity) const
{
    ColliderComponent *myCollider = myEntity.GetComponent<ColliderComponent>();

    for (auto &oppEntity : entities)
    {
        if (oppEntity->name.compare(myEntity.name) != 0 && oppEntity->name.compare("Tile") != 0)
        {
            if (oppEntity->HasComponent<ColliderComponent>())
            {
                ColliderComponent *otherCollider = oppEntity->GetComponent<ColliderComponent>();
                if (Collision::CheckRectangleCollision(myCollider->collider, otherCollider->collider))
                    return otherCollider->colliderTag;
            }
        }
    }

    return std::string();
}

//Check collisions against all entities
CollisionType EntityManager::CheckCollisions() const
{
    for (int i = 0; i < entities.size() - 1; i++)
    {
        auto &thisEntity = entities[i];
        if (thisEntity->HasComponent<ColliderComponent>())
        {
            ColliderComponent *thisCollider = thisEntity->GetComponent<ColliderComponent>();

            //j is equal to the "next" entity in the list
            for (int j = i + 1; j < entities.size(); j++)
            {
                auto &thatEntity = entities[j];
                if (thisEntity->name.compare(thatEntity->name) != 0 && thatEntity->HasComponent<ColliderComponent>())
                {
                    ColliderComponent *thatCollider = thatEntity->GetComponent<ColliderComponent>();

                    //If a collision happens check what type of collision it was
                    if (Collision::CheckRectangleCollision(thisCollider->collider, thatCollider->collider))
                    {
                        if (thisCollider->colliderTag.compare("PLAYER") == 0 &&
                            thatCollider->colliderTag.compare("ENEMY") == 0)
                        {
                            return PLAYER_ENEMY_COLLISION;
                        }
                        else if (thisCollider->colliderTag.compare("PLAYER") == 0 && thatCollider->colliderTag.compare("PROJECTILE") == 0)
                        {
                            return PLAYER_PROJECTILE_COLLISION;
                        }
                        else if (thisCollider->colliderTag.compare("ENEMY") == 0 && thatCollider->colliderTag.compare("FRIENDLY_PROJECTILE") == 0)
                        {
                            return ENEMY_PROJECTILE_COLLISION;
                        }
                        else if (thisCollider->colliderTag.compare("PLAYER") == 0 && thatCollider->colliderTag.compare("LEVEL_COMPLETE") == 0)
                        {
                            return PLAYER_LEVEL_COMPLETE_COLLISION;
                        }
                        else if (thisCollider->colliderTag.compare("WAYPOINT") == 0 && thatCollider->colliderTag.compare("ENEMY") == 0)
                        {
                            return ENEMY_WAYPOINT_COLLISION;
                        }
                    }
                }
            }
        }
    }

    return NO_COLLISION;
}

/*
Better alternative to collision checking:

Have each entity check for collisions each frame, but only against entities from selected layers. If a collision occurs, send a signal that it's happened
*/