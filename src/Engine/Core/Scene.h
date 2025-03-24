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
    std::unordered_map<std::string, Entity*> _handlers;
    std::unordered_set<Entity*> _entities;
    std::unordered_set<Entity*> _entitiesToAdd;
    std::map<int, std::unordered_set<RenderComponent*>> _renderComponents;
public:
    Scene();
    bool init();
    bool update() const;
    bool fixedUpdate() const;
    bool render(RenderManager* manager);
    void refresh();
    void addEntity(Entity* entity);
    void addHandler(Entity* entity, const std::string & handler);
    Entity* getEntityByHandler(const std::string & handler);
    void registerRenderComponent(RenderComponent* component , int layer);
    void unregisterRenderComponent(RenderComponent* component, int layer);
};


#endif //SCENE_H
