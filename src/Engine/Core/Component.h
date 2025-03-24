#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;
class Scene;
class Game;
class ComponentData;

class Component {
protected:
    bool _enabled;
    Entity* _entity;
    Scene* _scene;
    Game* _game;
    ComponentData* _data;
public:
    Component(ComponentData* data);
    virtual ~Component() = default;
    void setContext(Entity* entity, Scene* scene, Game* game);
    virtual bool init();
    virtual int getOrder() const;
    virtual bool update();
    virtual bool fixedUpdate();
    virtual void onEnable();
    virtual void onDisable();
    bool isEnabled() const;
    bool isEntityActive() const;
    void setEnabled(bool enabled);
    void initEnable();
};

#endif //COMPONENT_H
