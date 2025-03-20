#ifndef COMPONENT_H
#define COMPONENT_H
#include <string>

#include "Entity.h"
#include "Scene.h"
#include "SceneAPI.h"

class Component {
protected:
    bool _enabled;
    Entity* _entity;
    Scene* _scene;
    SceneAPI* _sceneManager;
public:
    virtual ~Component() = default;
    Component();
    void setContext(Entity* entity, Scene* scene, SceneAPI* sceneManager, bool enabled);
    virtual bool init() = 0;
    virtual bool update() = 0;
    virtual bool fixedUpdate() = 0;
    virtual void onEnable() = 0;
    virtual void onDisable() = 0;
    bool isEnabled() const;
    void setEnabled(bool enabled);
};

#endif //COMPONENT_H
