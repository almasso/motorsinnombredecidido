#include "MapComponent.h"
#include "OverworldManager.h"
#include <Collisions/Collider.h>
#include <Core/ComponentData.h>
#include <Core/Entity.h>
#include <Core/Scene.h>
#include <Utils/RPGError.h>

MapComponent::MapComponent(const ComponentData *data) :
    ComponentTemplate(data),
    _manager(nullptr),
    _collider(nullptr),
    _player(nullptr) {
}

bool MapComponent::init() {
    _adjacentMaps = _data->getSet("adjacentMaps");
    _collider = _entity->getComponent<Collider>();
    if (_collider == nullptr) {
        RPGError::ShowError("Map object sin Collider", "Todos los Mapas requieren de un componente Collider para funcionar");
        return false;
    }
    Entity* ent = _scene->getEntityByHandler("Manager");
    if (ent == nullptr || !ent->getComponent<OverworldManager>()) {
        RPGError::ShowError("No hay OverworldManager en escena", "Todos los mapas requieren de un OverworldManager para funcionar");
        return false;
    }
    _manager = ent->getComponent<OverworldManager>();
    ent = _scene->getEntityByHandler("Player");
    if (ent == nullptr || !ent->getComponent<Collider>()) {
        RPGError::ShowError("No hay jugador en escena", "Todos los mapas requieren de un jugador para funcionar");
        return false;
    }
    _player = ent->getComponent<Collider>();
    return true;
}

bool MapComponent::update() {
    if (_collider->justCollidedWith(_player)) {
        return _manager->changeMap(_adjacentMaps);
    }
    return true;
}

const std::unordered_set<std::string>& MapComponent::getAdjacentMaps() const {
    return _adjacentMaps;
}
