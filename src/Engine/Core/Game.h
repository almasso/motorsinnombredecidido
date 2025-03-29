#ifndef GAME_H
#define GAME_H
#include <string>
#include <sol/forward.hpp>

class SceneManager;
class Scene;
class Entity;

class Game {
private:
    SceneManager* _manager;
public:
    explicit Game(SceneManager* manager);
    Entity* instantiatePrefab(const std::string& handler) const;
    Scene* addScene(const std::string& handler) const;
    void popScene() const;

    static void RegisterToLua(sol::state& lua);
};


#endif //GAME_H
