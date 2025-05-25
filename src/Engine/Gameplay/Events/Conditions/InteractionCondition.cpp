#include "InteractionCondition.h"

#include <Collisions/Collider.h>
#include <Core/Entity.h>
#include <Core/Scene.h>
#include <Input/InputManager.h>
#include <Render/Camera.h>
#include <sol/table.hpp>

bool InteractionCondition::clickInside(const Rect &rect, const Vector2 &clickPos) {
    return (clickPos.getX() >= rect.x &&
            clickPos.getX() <= rect.x + rect.w &&
            clickPos.getY() >= rect.y &&
            clickPos.getY() <= rect.y + rect.h);
}

InteractionCondition::InteractionCondition() :
    _interactionArea(nullptr),
    _player(nullptr),
    _camera(nullptr) {
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

    _player = player->getComponent<Collider>();
    if (_player == nullptr)
        return false;

    Entity* camera = _scene->getEntityByHandler("Camera");
    if (camera == nullptr)
        return false;
    _camera = camera->getComponent<Camera>();
    if (_camera == nullptr)
        return false;

    return true;
}

bool InteractionCondition::met() {
    return _interactionArea->isCollidingWith(_player)
    && InputManager::GetState().mouse_down
    && clickInside(_interactionArea->getRect(), _camera->screenToWorld({InputManager::GetState().mouse_x, InputManager::GetState().mouse_y}));
}

InteractionCondition::~InteractionCondition() {
    _interactionArea = nullptr;
    _player = nullptr;
}
