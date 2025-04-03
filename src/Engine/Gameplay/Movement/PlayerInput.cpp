#include "PlayerInput.h"
#include "MovementComponent.h"
#include <Core/Scene.h>
#include <Core/Entity.h>
#include <Render/Camera.h>
#include <Utils/RPGError.h>
#include <Input/InputManager.h>

PlayerInput::PlayerInput(ComponentData const *data) :
    ComponentTemplate(data),
    _camera(nullptr),
    _movement(nullptr),
    _active(false){
}

bool PlayerInput::init() {
    _movement = _entity->getComponent<MovementComponent>();
    if (_movement == nullptr) {
        RPGError::ShowError("Player sin MovementComponent", "El player requiere de un componente MovementComponent para funcionar");
        return false;
    }
    if (Entity* ent = _scene->getEntityByHandler("Manager"); ent) {
        _camera = ent->getComponent<Camera>();
    }
    _active = true;
    return true;
}

bool PlayerInput::update() {
    if (_active) {
        InputState input = InputManager::GetState();
        if (input.mouse_down) {
            Vector2 pos = Vector2(input.mouse_x, input.mouse_y);
            if (_camera) pos = _camera->screenToWorld(pos);
            _movement->setTarget(pos);
        }
    }
    return true;
}

void PlayerInput::setActive(bool active) {
    _active = active;
}
