#include "Collider.h"

#include <Core/ComponentData.h>
#include <Core/Entity.h>
#include <Render/Transform.h>
#include <sol/state.hpp>
#include <sol/usertype.hpp>

#include "CollisionManager.h"

Collider::Collider(ComponentData const* data) :
    ComponentTemplate(data),
    _transform(nullptr) {
}

Collider::~Collider() {
    CollisionManager::Instance()->unregisterCollider(this);
}

bool Collider::init() {
    _transform = _entity->getComponent<Transform>();
    CollisionManager::Instance()->registerCollider(this);

    _pos = _data->getVector("position");
    _size = _data->getVector("size");

    return true;
}

void Collider::onEnable() {
    CollisionManager::Instance()->registerCollider(this);
}

void Collider::onDisable() {
    CollisionManager::Instance()->unregisterCollider(this);
}

bool Collider::isCollidingWith(Collider* other) const {
    return _isColliding.contains(other);
}

bool Collider::justCollidedWith(Collider* other) const {
    return _justCollided.contains(other);
}

bool Collider::collisionEndedWith(Collider* other) const {
    return _collisionEnded.contains(other);
}

Rect Collider::getRect() const {
    Rect r;
    auto pos = _transform->getGlobalPosition();
    auto scale = _transform->getGlobalScale();
    r.w = scale.getX() * _size.getX();
    r.h = scale.getY() * _size.getY();
    r.x = pos.getX() + _pos.getX() * scale.getX() - r.w / 2.f;
    r.y = pos.getY() + _pos.getY() * scale.getY() - r.h / 2.f;
    return r;
}

void Collider::RegisterToLua(sol::state& luaState) {
    sol::usertype<Collider> type = luaState.new_usertype<Collider>("Collider");
    type["isCollidingWith"] = &Collider::isCollidingWith;
    type["justCollidedWith"] = &Collider::justCollidedWith;
    type["collisionEndedWith"] = &Collider::collisionEndedWith;
    type["get"] = Collider::get;
}
