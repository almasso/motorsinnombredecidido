#ifndef ENTITY_H
#define ENTITY_H
#include <map>
#include <unordered_set>

class RenderManager;
class Component;

class Entity {
private:
    std::map<int, Component*> _components;
    std::unordered_set<Entity*> _children;
    Entity* _parent;
    bool _active;
    bool _alive;
public:
    Entity();
    bool update();
    bool fixedUpdate();
    bool isActive() const;
    void setActive(bool active);
    bool isAlive() const;
    void destroy();
    void addChild(Entity* child);
    void removeChild(Entity* child);
    Entity* getParent() const;

    template<class ComponentType>
    bool addComponent(Component* component) {
        if (_components.insert({ComponentType::order, component}).second) {
            return true;
        }
        return false;
    }

    template<class ComponentType>
    ComponentType* getComponent() {
        auto finder = _components.find(ComponentType::order);
        if (finder == _components.end()) {
            return nullptr;
        }
        return static_cast<ComponentType*>(finder->second);
    }

};
#endif //ENTITY_H
