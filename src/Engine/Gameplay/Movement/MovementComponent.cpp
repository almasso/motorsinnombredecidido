#include "MovementComponent.h"
#include "MovementManager.h"
#include <Core/ComponentData.h>
#include <Core/Entity.h>
#include <Render/Transform.h>
#include <Utils/Time.h>
#include <sol/state.hpp>

MovementComponent::MovementComponent(ComponentData const* data) :
    ComponentTemplate(data),
    _speed(1.0f),
    _pathIndex(0) {
}

bool MovementComponent::init() {
    if (!MovementObstacle::init()) {
        return false;
    }
    _speed = _data->getData<float>("speed", 4.0f);
    return true;
}

void MovementComponent::setTarget(const Vector2& target) {
    _path = _manager->calculatePath(_transform->getGlobalPosition(), target);
    _pathIndex = 0;
}

bool MovementComponent::update() {
    if (_path.empty() || _pathIndex >= _path.size()) {
        return true;
    }
    Vector2 currentPosition = _transform->getGlobalPosition();
    Vector2 nextPosition = _path[_pathIndex];

    Vector2 direction = (nextPosition - currentPosition).normalized();
    Vector2 movement = direction * _speed * Time::deltaTime;

    if ((nextPosition - currentPosition).magnitude() <= movement.magnitude()) {
        _transform->setPosition(nextPosition);
        _pathIndex++;
        _manager->unregisterObstacle(_iterator);
        _iterator = _manager->registerObstacle(_transform->getGlobalPosition());
        if (_manager->isOccupied(_path[_pathIndex])) {
            _path = _manager->calculatePath(_transform->getGlobalPosition(), _path[_path.size()-1]);
            _pathIndex = 0;
        }
    } else {
        _transform->move(movement);
    }
    return true;
}

void MovementComponent::RegisterToLua(sol::state& lua) {
    sol::usertype<MovementComponent> type = lua.new_usertype<MovementComponent>("MovementComponent");
    type["setTarget"] = &MovementComponent::setTarget;
    type["get"] = MovementComponent::get;
}
