#ifndef SCENEAPI_H
#define SCENEAPI_H
#include <string>

class SceneManager;
class Scene;
class Entity;

class SceneAPI {
private:
    SceneManager* manager;
public:
    SceneAPI(SceneManager* manager);
    Entity* instanciatePrefab(std::string handler);
    Scene* addScene(std::string handler);
    void popScene();
};


#endif //SCENEAPI_H
