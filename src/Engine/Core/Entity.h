#ifndef ENTITY_H
#define ENTITY_H
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>

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
};


#endif //ENTITY_H
