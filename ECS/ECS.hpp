#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
using namespace std;

class Component;
class Entity;
class Manager;

using ComponentID = size_t;
using Group = size_t;
constexpr ComponentID maxComponents = 32;
constexpr Group maxGroups = 32;

using ComponentBitSet = bitset<maxComponents>;
using GroupBitSet = bitset<maxGroups>;
using ComponentArray = array<Component *, maxComponents>;

inline ComponentID getComponentTypeID()
{
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getComponentTypeID();
    return typeID;
}

class Component
{
public:
    Entity *entity;
    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}
    virtual ~Component() {}
};

class Entity
{
private:
    Manager &manager;
    bool active = true;
    vector<unique_ptr<Component>> components;
    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
    GroupBitSet groupBitSet;

public:
    Entity(Manager &mManager) : manager(mManager) {}

    void update()
    {
        for (auto &c : components)
            c->update();
        // for (auto &c : components)
        //     c->draw();
    }
    void draw()
    {
        for (auto &c : components)
            c->draw();
    }
    bool isActive() const { return active; }

    void destroy() { active = false; }

    bool hasGroup(Group mGroup)
    {
        return groupBitSet[mGroup];
    }
    void addGroup(Group mGroup);
    void delGroup(Group mGroup)
    {
        groupBitSet[mGroup] = false;
    }
    template <typename T>
    bool hasComponent() const
    {
        return componentBitSet[getComponentTypeID<T>()];
    }
    // gari.addComponent<TransformComponent>()
    template <typename T, typename... TArgs>
    T &addComponent(TArgs &&...mArgs)
    {
        T *c(new T(forward<TArgs>(mArgs)...));
        c->entity = this;
        unique_ptr<Component> uPtr{c};
        components.emplace_back(move(uPtr));

        componentArray[getComponentTypeID<T>()] = c;
        componentBitSet[getComponentTypeID<T>()] = true;
        c->init();
        return *c;
    }
    template <typename T>
    T &getComponent() const
    {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T *>(ptr);
    }
};

class Manager
{
private:
    vector<unique_ptr<Entity>> entities;
    array<vector<Entity *>, maxGroups> groupedEntities;

public:
    void update()
    {
        for (auto &e : entities)
            e->update();
    }
    void draw()
    {
        for (auto &e : entities)
            e->draw();
    }
    void refresh()
    {
        for (auto i = 0u; i < maxGroups; i++)
        {
            auto &v(groupedEntities[i]);
            v.erase(
                remove_if(begin(v), end(v), 
                [i](Entity *mEntity) { return !mEntity->isActive() || !mEntity->hasGroup(i); }
                ),
                end(v)
                );
        }
        entities.erase(
            remove_if(begin(entities), end(entities),
                      [](const unique_ptr<Entity> &mEntity)
                      { return !mEntity->isActive(); }),
            end(entities));
    }
    void addToGroup(Entity *mEntity, Group mGroup)
    {
        groupedEntities[mGroup].emplace_back(mEntity);
    }
    vector<Entity *> &getGroup(Group mGroup)
    {
        return groupedEntities[mGroup];
    }

    Entity &addEntity()
    {
        Entity *e = new Entity(*this);
        unique_ptr<Entity> uPtr{e};
        entities.emplace_back(move(uPtr));
        return *e;
    }
};
