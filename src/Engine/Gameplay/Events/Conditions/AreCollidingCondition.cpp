#include "AreCollidingCondition.h"

#include <Collisions/Collider.h>
#include <Core/Entity.h>
#include <Core/Scene.h>
#include <sol/table.hpp>
#include <Utils/Error.h>

bool AreCollidingCondition::initCollider(Collider*& collider, std::string const& handler) {
    Entity* entityA = _scene->getEntityByHandler(handler);
    if (!entityA)
        return false;
    collider = entityA->getComponent<Collider>();
    if (collider == nullptr) {
        Error::ShowError("Collision check error", "Object \"" + handler + "\" has no collisions");
    }
    return collider != nullptr;
}

AreCollidingCondition::AreCollidingCondition() :
    _colliderA(nullptr),
    _colliderB(nullptr) {
}

bool AreCollidingCondition::init(sol::table const& params) {
    if (!params.valid())
        return false;

    std::string colliderA = params.get_or<std::string>("colliderA", "");
    if (colliderA.empty())
        return false;

    if (!initCollider(_colliderA, colliderA))
        return false;

    std::string colliderB = params.get_or<std::string>("colliderB", "");
    if (colliderB.empty())
        return false;

    if (!initCollider(_colliderB, colliderB))
        return false;

    return true;
}

bool AreCollidingCondition::met() {
    return _colliderA->isCollidingWith(_colliderB);
}

AreCollidingCondition::~AreCollidingCondition() {
    _colliderA = nullptr;
    _colliderB = nullptr;
}
