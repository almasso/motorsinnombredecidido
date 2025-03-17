#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <list>
#include <string>

class RenderManager;
class Entity;
class Scene;
class SceneAPI;

class PrefabBlueprint;
class SceneBlueprint;

class SceneManager {
    private:
    static SceneManager* instance;
    std::list<Scene*> scenes;
    SceneAPI* api;
    SceneManager();
    ~SceneManager();
    Entity* createPrefab(std::string handler);
    Scene* createScene(std::string handler);
    
    public:
    static SceneManager* Init();
    bool update();
    bool fixedUpdate();
    bool render(RenderManager* render);
    void refresh();
    void shutdown();
    Entity* instanciatePrefab(std::string handler);
    Scene* addScene(std::string handler);
    void popScene();
};

#endif //SCENEMANAGER_H
