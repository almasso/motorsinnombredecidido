//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "AreCollidingCondition.h"

#define objectAKey "objectA"
#define objectBKey "objectB"

editor::resources::events::AreCollidingCondition::AreCollidingCondition() :
    _objectA(),
    _objectB() {
}

editor::resources::events::AreCollidingCondition::~AreCollidingCondition() = default;

bool editor::resources::events::AreCollidingCondition::read(sol::table const& params) {
    _objectA = params.get_or<std::string>(objectAKey, "");
    if (_objectA.empty())
        return false;

    _objectB = params.get_or<std::string>(objectBKey, "");
    if (_objectB.empty())
        return false;
    return true;
}

bool editor::resources::events::AreCollidingCondition::render() {
    return false;
}

bool editor::resources::events::AreCollidingCondition::writeParamsToEngine(sol::table& params) {
    params[objectAKey] = _objectA;
    params[objectBKey] = _objectB;
    return true;
}

bool editor::resources::events::AreCollidingCondition::writeParams(sol::table& params) {
    params[objectAKey] = _objectA;
    params[objectBKey] = _objectB;
    return true;
}

