#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <list>
#include <string>

class RenderManager;
class Entity;
class Scene;
class Game;
class ComponentFactory;

class ComponentData;
class EntityBlueprint;
class SceneBlueprint;

class SceneManager {
    private:
    static SceneManager* _instance;
    std::list<Scene*> _scenes;
    Game* _api;
    ComponentFactory* _factory;
    SceneManager();
    ~SceneManager();
    Entity* createEntity(const std::string& handler);
    Scene* createScene(const std::string& handler);
    
    public:
    static SceneManager* Init();
    bool update() const;
    bool fixedUpdate() const;
    bool render(RenderManager* render) const;
    void refresh() const;
    void shutdown() const;
    Entity* instantiatePrefab(const std::string& handler);
    Scene* addScene(const std::string& handler);
    void popScene();
};

#endif //SCENEMANAGER_H
