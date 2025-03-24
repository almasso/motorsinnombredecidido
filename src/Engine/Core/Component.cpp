#include "Component.h"
#include "ComponentData.h"
#include "Entity.h"

Component::Component(ComponentData* data) : _enabled(false) ,_entity(nullptr),  _scene(nullptr), _game(nullptr), _data(data) {
}

void Component::setContext(Entity *entity, Scene *scene, Game *game) {
    _entity = entity;
    _scene = scene;
    _game = game;
}

bool Component::init() {
    return true;
}

int Component::getOrder() const{
    return -1;
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

void Component::initEnable() {
    setEnabled(_data->getData("enabled", true));
}
