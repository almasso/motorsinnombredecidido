#include "Component.h"
#include "Entity.h"

Component::Component() : _enabled(false),  _entity(nullptr), _scene(nullptr), _game(nullptr) {
}

void Component::setContext(Entity *entity, Scene *scene, Game *game) {
    _entity = entity;
    _scene = scene;
    _game = game;
}

bool Component::init() {
    return true;
}

bool Component::update() {
    return true;
}

bool Component::fixedUpdate() {
    return true;
}

void Component::onEnable() {

}

void Component::onDisable() {
}

bool Component::isEnabled() const {
    return _enabled;
}

bool Component::isEntityActive() const {
    return _entity->isActive();
}

void Component::setEnabled(const bool enabled) {
    if (_enabled != enabled) {
        _enabled = enabled;
        if (_enabled) onEnable();
        else onDisable();
    }
}
