#ifndef ENTITY_H
#define ENTITY_H
#include <map>
#include <unordered_set>
#include "Component.h"
#include <sol/forward.hpp>

class RenderManager;
class Component;

class Entity {
private:
    std::map<int, Component*> _components;
    std::map<std::string, Component*> _componentIds;
    std::unordered_set<Entity*> _children;
    Entity* _parent;
    bool _active;
    bool _alive;
public:
    Entity();
    bool init();
    bool update();
    bool fixedUpdate();
    bool isActive() const;
    void setActive(bool active);
    bool isAlive() const;
    void destroy();
    bool addChild(Entity* child);
    void removeChild(Entity* child);
    void setParent(Entity* parent);
    Entity* getParent() const;

    bool addComponent(Component *component, const std::string& id);
    Component* getComponent(const std::string& componentID);

    template<class ComponentType>
    ComponentType* getComponent() {
        auto finder = _components.find(ComponentType::order);
        if (finder == _components.end()) {
            return nullptr;
        }
        return static_cast<ComponentType*>(finder->second);
    }


    static void RegisterToLua(sol::state& lua);
};
#endif //ENTITY_H
