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
    sol::optional<std::string> objectA = params.get<sol::optional<std::string>>(objectAKey);
    if (!objectA.has_value())
        return false;
    _objectA = objectA.value();

    sol::optional<std::string> objectB = params.get<sol::optional<std::string>>(objectBKey);
    if (!objectB.has_value())
        return false;
    _objectB = objectB.value();

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

