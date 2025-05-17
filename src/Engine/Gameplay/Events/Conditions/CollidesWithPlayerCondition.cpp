#include "CollidesWithPlayerCondition.h"

#include <Collisions/Collider.h>
#include <Core/Entity.h>
#include <Core/Scene.h>
#include <sol/table.hpp>
#include <Utils/Error.h>

bool CollidesWithPlayerCondition::initCollider(Collider*& collider, Entity* entity) {
    collider = entity->getComponent<Collider>();
    return collider != nullptr;
}

CollidesWithPlayerCondition::CollidesWithPlayerCondition() :
    _collider(nullptr),
    _playerCollider(nullptr) {
}

bool CollidesWithPlayerCondition::init(sol::table const& params) {
    if (!initCollider(_collider, _entity)) {
        Error::ShowError("Collision check error", "Object has no collisions");
        return false;
    }

    Entity* player = _scene->getEntityByHandler("Player");
    if (player == nullptr) {
        Error::ShowError("Collision check error", "Player entity isn't on scene.");
        return false;
    }

    if (!initCollider(_playerCollider, player)) {
        Error::ShowError("Collision check error", "Player has no collisions");
        return false;
    }

    return true;
}

bool CollidesWithPlayerCondition::met() {
    return _collider->isCollidingWith(_playerCollider);
}

CollidesWithPlayerCondition::~CollidesWithPlayerCondition() {
    _collider = nullptr;
    _playerCollider = nullptr;
}
