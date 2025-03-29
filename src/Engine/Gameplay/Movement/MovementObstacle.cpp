#include "MovementObstacle.h"
#include "MovementManager.h"
#include <Render/Transform.h>
#include <Core/Scene.h>
#include <Core/Entity.h>
#include <Utils/RPGError.h>

MovementObstacle::MovementObstacle(ComponentData const* data) :
ComponentTemplate(data),
_transform(nullptr),
_manager(nullptr) {
}

bool MovementObstacle::init() {
    _transform = _entity->getComponent<Transform>();
    if (_transform == nullptr) {
        RPGError::ShowError("MovementObstacle object sin transform", "Todos los objetos con movimiento requieren de un componente Transform para funcionar");
        return false;
    }
    Entity* ent = _scene->getEntityByHandler("Manager");
    if (ent == nullptr || !ent->getComponent<MovementManager>()) {
        RPGError::ShowError("No hay MovementManager en escena", "Todos los objetos con movimiento requieren de un MovementManager para funcionar");
        return false;
    }
    _manager = ent->getComponent<MovementManager>();
    return true;
}

void MovementObstacle::onEnable() {
    _iterator = _manager->registerObstacle(_transform->getGlobalPosition());
}

void MovementObstacle::onDisable() {
    _manager->unregisterObstacle(_iterator);
}
