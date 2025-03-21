#ifndef SCENEAPI_H
#define SCENEAPI_H
#include <string>

class SceneManager;
class Scene;
class Entity;

class Game {
private:
    SceneManager* _manager;
public:
    explicit Game(SceneManager* manager);
    Entity* instantiatePrefab(const std::string& handler) const;
    Scene* addScene(std::string handler) const;
    void popScene() const;
};


#endif //SCENEAPI_H
