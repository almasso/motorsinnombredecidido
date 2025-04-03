#include "Button.h"

#include <Core/ComponentData.h>
#include <Render/Transform.h>
#include <Utils/Error.h>

#include "InputManager.h"

Button::Button(ComponentData const *data) : ComponentTemplate(data), _transform(nullptr) {
}

bool Button::init() {
    _transform = _entity->getComponent<Transform>();
    if (_transform == nullptr) {
        Error::ShowError("Button object sin transform", "Todos los botones requieren de un componente Transform para funcionar");
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
            Vector2(input.mouse_x, input.mouse_y))) {
            return _callback(_params);
        }
    }
    return true;
}

