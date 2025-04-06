//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "MoveBehaviour.h"

#define xTargetKey "xTarget"
#define yTargetKey "yTarget"

editor::resources::events::MoveBehaviour::MoveBehaviour() :
    _xTarget(0),
    _yTarget(0) {
}

editor::resources::events::MoveBehaviour::~MoveBehaviour() {
}

bool editor::resources::events::MoveBehaviour::read(sol::table const& params) {
    sol::optional<int> xTarget = params.get<sol::optional<int>>(xTargetKey);
    if (!xTarget.has_value())
        return false;
    _xTarget = xTarget.value();

    sol::optional<int> yTarget = params.get<sol::optional<int>>(yTargetKey);
    if (!yTarget.has_value())
        return false;
    _yTarget = yTarget.value();

    return true;
}

bool editor::resources::events::MoveBehaviour::writeToEngine(sol::table& behaviour) {
    return true;
}

bool editor::resources::events::MoveBehaviour::writeParams(sol::table& params) {
    params[xTargetKey] = _xTarget;
    params[yTargetKey] = _yTarget;
    return true;
}
