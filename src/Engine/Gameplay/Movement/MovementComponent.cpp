#include "MovementComponent.h"
#include "MovementManager.h"
#include <Core/ComponentData.h>
#include <Core/Entity.h>
#include <Render/Transform.h>
#include <Utils/Time.h>
#include <Render/Animator.h>
#include <sol/state.hpp>
#include <Utils/Error.h>


MovementComponent::MovementComponent(ComponentData const* data) :
    ComponentTemplate(data),
    _speed(1.0f),
    _pathIndex(0),
    _animator(nullptr){
}

bool MovementComponent::init() {
    if (!MovementObstacle::init()) {
        return false;
    }
    _animator = _entity->getComponent<Animator>();
    _speed = _data->getData<float>("speed", 4.0f);
    _animations = _data->getArray<std::string,4>("animations");
    return true;
}

void MovementComponent::setTarget(const Vector2& target) {
    _path = _manager->calculatePath(_transform->getGlobalPosition(), target);
    _pathIndex = 0;
}

bool MovementComponent::update() {
    if (_path.empty()) {
        return true;
    }
    if (_pathIndex >= _path.size()) {
        _path.clear();
        if (_animator) _animator->changeAnimation(std::string());
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
        if (_animator) {
            std::string newAnimation;
            if (std::abs(direction.getX()) > std::abs(direction.getY())) {
                newAnimation = direction.getX() > 0 ? _animations[3] : _animations[2];
            } else {
                newAnimation = direction.getY() > 0 ? _animations[1] : _animations[0];
            }
            _animator->changeAnimation(newAnimation);
        }
    }
    return true;
}

void MovementComponent::RegisterToLua(sol::state& lua) {
    sol::usertype<MovementComponent> type = lua.new_usertype<MovementComponent>("MovementComponent");
    type["setTarget"] = &MovementComponent::setTarget;
    type["get"] = MovementComponent::get;
}
