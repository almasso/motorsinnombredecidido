#include "Component.h"

Component::Component() : _enabled(false),  _entity(nullptr), _scene(nullptr), _sceneManager(nullptr) {
}

void Component::setContext(Entity *entity, Scene *scene, SceneAPI *sceneManager, const bool enabled) {
    _entity = entity;
    _scene = scene;
    _sceneManager = sceneManager;
    setEnabled(enabled);
}

bool Component::isEnabled() const {
    return _entity->isActive() && _enabled;
}

void Component::setEnabled(const bool enabled) {
    if (_enabled != enabled) {
        _enabled = enabled;
        if (_enabled) onEnable();
        else onDisable();
    }
}
