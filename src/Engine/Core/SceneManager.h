#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <list>
#include <string>


class RenderManager;
class Component;
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
    Game* _game;
    ComponentFactory* _factory;
    SceneManager();
    ~SceneManager();
    Entity* createEntity(const EntityBlueprint *blueprint, Scene *scene);
    Scene* createScene(const SceneBlueprint *blueprint);
    bool init(const std::string& startScene);

    public:
    static SceneManager* Init(const std::string& startScene);
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
