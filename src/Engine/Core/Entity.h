#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <unordered_map>

class RenderManager;
class Component;

class Entity {
private:
    std::unordered_map<std::string, Component*> components;
    bool enabled;
public:
    Entity();
    bool update();
    bool fixedUpdate();
    bool render(RenderManager* manager);
    bool isEnabled();
    void setEnabled();
    Component* getComponent();
    Component* addComponent(Component* component);
    void removeComponent(Component* component);

};

#endif //ENTITY_H
