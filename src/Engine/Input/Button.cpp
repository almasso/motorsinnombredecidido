#include "Button.h"

#include <Core/ComponentData.h>
#include <Core/Scene.h>
#include <Render/Camera.h>
#include <Render/Transform.h>
#include <Utils/Error.h>

#include "InputManager.h"

Button::Button(ComponentData const *data) : ComponentTemplate(data), _transform(nullptr), _camera(nullptr) {
}

bool Button::init() {
    _transform = _entity->getComponent<Transform>();
    if (_transform == nullptr) {
        Error::ShowError("Button object sin transform", "Todos los botones requieren de un componente Transform para funcionar");
        return false;
    }
    if (Entity* ent = _scene->getEntityByHandler("Camera"); ent) {
        _camera = ent->getComponent<Camera>();
    }
    if (_camera == nullptr) {
        Error::ShowError("Button object sin camara en escena", "Todos los botones requieren de un componente Camera en escena");
        return false;
    }
    _callback = _data->getData<sol::function>("callback");
    _params = _data->getData<sol::table>("params");
    _size = _data->getVector("size", Vector2::UNIT);
    return true;
}


bool clickInside(const Vector2 &pos, const Vector2 &size, const Vector2 &clickPos) {
    return (clickPos.getX() >= pos.getX() - size.getX() / 2 &&
            clickPos.getX() <= pos.getX() + size.getX() / 2 &&
            clickPos.getY() >= pos.getY() - size.getY() / 2 &&
            clickPos.getY() <= pos.getY() + size.getY() / 2);
}

bool Button::update() {
    InputState input = InputManager::GetState();
    if (input.mouse_down) {
        if (clickInside(_transform->getGlobalPosition(),
            _transform->getGlobalScale() * _size,
            _camera->screenToWorld(Vector2(input.mouse_x, input.mouse_y)))) {
            return _callback(_params);
        }
    }
    return true;
}

void Button::setCallback(const sol::function &callback) {
    _callback = callback;
}

void Button::RegisterToLua(sol::state& lua) {
    sol::usertype<Button> type = lua.new_usertype<Button>("Button");
    type["setCallback"] = &Button::setCallback;
    type["get"] = &Button::get;
}


