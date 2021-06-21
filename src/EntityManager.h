#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include "./Entity.h"
#include "./Components/Component.h"

//The EntityManager has a list of all the entities
class EntityManager
{
private:
    std::vector<Entity *> entities;
    std::vector<Entity *> waypoints;

public:
    void ClearData();
    void Update(float deltaTime);
    void Render();
    bool HasNoEntities();
    Entity &AddEntity(std::string entityName, LayerType layer);
    std::vector<Entity *> GetEntities() const;
    std::vector<Entity *> GetEntitiesByLayer(LayerType layer) const;
    std::vector<Entity *> GetWaypoints() const;
    unsigned int GetEntityCount();

    std::string CheckEntityCollisions(Entity &entity) const;
    CollisionType CheckCollisions() const;

    void DestroyInactiveEntities();

    void ListAllEntities();

    Entity &GetPlayerAddress();
};

#endif

/*
So if we have the #ifndef preprocessor directive, which makes conditional compilation, then why not just secretly include all header files? In fact, why not include ./*.h or something - since each header file can only be included once?
*/