//If this header is not defined - define it
//These are called Header Guards. If ENTITY_H is not defined, define it and run the code in this file - the header is only run once

#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <iostream>
#include <map>

#include "./Constants.h"
#include "./EntityManager.h"
#include "./Components/Component.h"

//Forward Declarations
class Component;
class EntityManager;
//The compiler needs to know that these classes exist before using them in the Entity class
/*
Ok so Forward Declarations are used whenever Cyclic Dependency occurs. The Component class has an Entity reference called owner, and the Entity has a reference to a list of Components. One has to be declared first, but the code won't compile unless the substitute, empty classes are made - this is a concept unique to C++
*/

class Entity
{
private:
    EntityManager &manager;
    bool isActive;
    std::vector<Component *> components;

    //Map that keeps track of Components by their type
    //type_info objects represent "types"
    std::map<const std::type_info *, Component *> componentTypeMap;

public:
    std::string name;
    LayerType layer;
    Entity(EntityManager &manager);
    Entity(EntityManager &manager, std::string entityName, LayerType layer);
    void Update(float deltaTime);
    void Render();
    void Destroy();
    bool IsActive() const;

    //AddComponent function returns a T object (Component) and takes TArgs
    //T = A generic variable, in this case a Component type. TArgs represents several of these
    //Create a new component of type T and add it to the component map and list
    template <typename T, typename... TArgs>
    T &AddComponent(TArgs &&...args)
    {
        //Create a new object (component) of type T with args forwarded into it
        T *newComponent(new T(std::forward<TArgs>(args)...));
        newComponent->owner = this;
        components.emplace_back(newComponent);

        //Extracts the type out of the component and uses that type as the "ID" for storage in the map
        componentTypeMap[&typeid(*newComponent)] = newComponent;

        newComponent->Initialize();
        return *newComponent;
    }

    /*
    C++ Template functions are not real functions, they are placeholders for actual functions with real, non-generic types
    */

    //Returns a component of type T
    template <typename T>
    T *GetComponent()
    {
        //Use the passed-in type as the ID for the type of component to look for in the map, then return that component by converting the type_info
        return static_cast<T *>(componentTypeMap[&typeid(T)]);
    }

    //Check if a component exists in the component map
    template <typename T>
    bool HasComponent() const
    {
        //Check how many components of type T exist in the map - by "extracting" the generic type out of T and using it as an ID
        return componentTypeMap.count(&typeid(T));
    }

    //Lists all of the components
    void ListAllComponents();
};

#endif
//The #ifndef ends here