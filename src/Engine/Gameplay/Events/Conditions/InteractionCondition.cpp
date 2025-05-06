#include "InteractionCondition.h"

#include <Collisions/Collider.h>
#include <Core/Entity.h>
#include <Core/Scene.h>
#include <Input/InputManager.h>
#include <sol/table.hpp>

InteractionCondition::InteractionCondition() :
    _interactionArea(nullptr),
    _player(nullptr) {
}

bool InteractionCondition::init(sol::table const& params) {
    if (!params.valid())
        return false;

    std::string interactionArea = params.get_or<std::string>("interactionArea", "");
    if (interactionArea.empty())
        return false;

    Entity* ent = _scene->getEntityByHandler(interactionArea);
    if (ent == nullptr)
        return false;

    _interactionArea = ent->getComponent<Collider>();
    if (_interactionArea == nullptr)
        return false;

    Entity* player = _scene->getEntityByHandler("Player");
    if (player == nullptr)
        return false;

    _player = ent->getComponent<Collider>();
    return _player != nullptr;
}

bool InteractionCondition::met() {
    return _interactionArea->isCollidingWith(_player) && InputManager::GetState().mouse_down;
}

InteractionCondition::~InteractionCondition() {
    _interactionArea = nullptr;
    _player = nullptr;
}
