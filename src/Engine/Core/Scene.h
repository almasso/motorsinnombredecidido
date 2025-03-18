#ifndef SCENE_H
#define SCENE_H
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>

class Entity;
class RenderComponent;
class RenderManager;

class Scene {
private:
    std::unordered_map<std::string, Entity*> handlers;
    std::unordered_set<Entity*> entities;
    std::map<int, std::unordered_set<RenderComponent*>> renderComponents;
public:
    Scene();
    bool update();
    bool fixedUpdate();
    bool render(RenderManager* manager);
    void refresh();
    void addEntity(Entity* entity, const std::string & handler);
    Entity* getEntityByHandler(const std::string & handler);
    void registerRenderComponent(RenderComponent* component , int layer);
    void unregisterRenderComponent(RenderComponent* component, int layer);
};


#endif //SCENE_H
