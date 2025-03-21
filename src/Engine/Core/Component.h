#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;
class Scene;
class Game;

class Component {
protected:
    bool _enabled;
    Entity* _entity;
    Scene* _scene;
    Game* _game;
public:
    Component();
    virtual ~Component() = default;
    void setContext(Entity* entity, Scene* scene, Game* game);
    virtual bool init();
    virtual bool update();
    virtual bool fixedUpdate();
    virtual void onEnable();
    virtual void onDisable();
    bool isEnabled() const;
    bool isEntityActive() const;
    void setEnabled(bool enabled);
};

#endif //COMPONENT_H
