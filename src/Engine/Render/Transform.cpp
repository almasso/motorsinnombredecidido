#include "Transform.h"
#include <Core/ComponentData.h>
#include <Core/Entity.h>

Transform::Transform(ComponentData const*data) : ComponentTemplate(data), _rotation(0) {
}

bool Transform::init() {
    _position = _data->getVector("position",{0,0});
    _rotation = _data->getData<float>("rotation",0);
    _scale = _data->getVector("scale",{1,1});
    return true;
}

const Vector2& Transform::getPosition() const {
    return _position;
}

Vector2 Transform::getGlobalPosition() const {
    if (Transform* parent = getParent()) {
        return _position + parent->getGlobalPosition();
    }
    return _position;
}

void Transform::setPosition(const Vector2 &position) {
    _position = position;
}

void Transform::setPosition(const float x, const float y) {
    _position = Vector2(x, y);
}

void Transform::setPositionX(const float x) {
    _position.setX(x);
}

void Transform::setPositionY(const float y) {
    _position.setY(y);
}

void Transform::move(const Vector2 &movement) {
    _position += movement;
}

void Transform::move(const float movX, const float movY) {
    _position += Vector2(movX, movY);
}

void Transform::moveX(const float movY) {
    _position.setX(_position.getX() + movY);
}

void Transform::moveY(const float movX) {
    _position.setY(_position.getY() + movX);
}

const Vector2& Transform::getScale() const {
    return _scale;
}

Vector2 Transform::getGlobalScale() const {
    if (Transform* parent = getParent()) {
        return _scale * parent->getGlobalScale();
    }
    return _scale;
}

void Transform::setScale(const Vector2 &scale) {
    _scale = scale;
}

void Transform::setScale(float x, float y) {
    _scale = Vector2(x, y);
}

void Transform::setScaleX(float x) {
    _scale.setX(x);
}

void Transform::setScaleY(float y) {
    _scale.setY(y);
}

void Transform::doScale(const Vector2 &scaleFactor) {
    _scale *= scaleFactor;
}

void Transform::doScale(const float scaleX, const float scaleY) {
    _scale *= Vector2(scaleX, scaleY);
}

void Transform::doScaleX(const float scaleX) {
    _scale.setX(_scale.getX() * scaleX);
}

void Transform::doScaleY(const float scaleY) {
    _scale.setX(_scale.getX() * scaleY);
}

const float& Transform::getRotation() const {
    return _rotation;
}

float Transform::getGlobalRotation() const {
    if (Transform* parent = getParent()) {
        return _rotation + parent->getGlobalRotation();
    }
    return _rotation;
}

void Transform::setRotation(const float rotation) {
    _rotation = rotation;
}

void Transform::rotate(const float rotation) {
    _rotation += rotation;
}

Transform* Transform::getParent() const {
    if (Entity* parent = _entity->getParent();
        parent != nullptr) {
        return parent->getComponent<Transform>();
    }
    return nullptr;
}

void Transform::RegisterToLua(sol::state& lua) {
    sol::usertype<Transform> type = lua.new_usertype<Transform>("Transform");

    type["position"] = &Transform::_position;
    type["getGlobalPosition"] = &Transform::getGlobalPosition;
    type["setPositionX"] = &Transform::setPositionX;
    type["setPositionY"] = &Transform::setPositionY;
    type["moveX"] = &Transform::moveX;
    type["moveY"] = &Transform::moveY;

    type["scale"] = &Transform::_scale;
    type["getGlobalScale"] = &Transform::getGlobalScale;
    type["setScaleX"] = &Transform::setScaleX;
    type["setScaleY"] = &Transform::setScaleY;
    type["doScaleX"] = &Transform::doScaleX;
    type["doScaleY"] = &Transform::doScaleY;

    type["rotation"] = sol::property(&Transform::getRotation, &Transform::setRotation);
    type["getGlobalRotation"] = &Transform::getGlobalRotation;
    type["rotate"] = &Transform::rotate;

    type["getParent"] = &Transform::getParent;
    type["get"] = Transform::get;
}
