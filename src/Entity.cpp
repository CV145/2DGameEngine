#include <iostream>
#include "./Entity.h"

Entity::Entity(EntityManager &manager) : manager(manager)
{
    this->isActive = true;
}

Entity::Entity(EntityManager &manager, std::string name, LayerType layer) : manager(manager), name(name), layer(layer)
{
    this->isActive = true;
}

//The Entity updates all of its components
void Entity::Update(float deltaTime)
{
    for (auto &component : components)
    {
        component->Update(deltaTime);
    }
}

//The Entity renders all of its components
//Or in a way: The Entity "pushes the render button" of each of its components
void Entity::Render()
{
    for (auto &component : components)
    {
        component->Render();
    }
}

void Entity::Destroy()
{
    this->isActive = false;
}

bool Entity::IsActive() const
{
    return this->isActive;
}

void Entity::ListAllComponents()
{
    for (auto mapElement : componentTypeMap)
    {
        std::cout << "\t"
                  << "Component<" << mapElement.first->name() << ">" << std::endl;
    }
}

/*
The auto keyword in C++ is used to "automatically" deduce a type - use it whenever you're lazy
*/