#ifndef COMPONENT_H
#define COMPONENT_H

#include "../Entity.h"

class Entity;

class Component
{
public:
    Entity *owner;
    virtual ~Component() {}
    virtual void Initialize() {}
    virtual void Update(float deltaTime) {}
    virtual void Render() {}
};

#endif

/*
Virtual Classes
- Classes that are going to be overriden
*/