//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "JumpBehaviour.h"

#define targetKey "target"

editor::resources::events::JumpBehaviour::JumpBehaviour() :
    _target(-1){
}

editor::resources::events::JumpBehaviour::~JumpBehaviour() = default;

bool editor::resources::events::JumpBehaviour::read(sol::table const& params) {
    sol::optional<int> target = params.get<sol::optional<int>>(targetKey);
    if (!target.has_value())
        return false;
    _target = target.value();
    return true;
}

bool editor::resources::events::JumpBehaviour::writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) {
    return true;
}

bool editor::resources::events::JumpBehaviour::render() {
    return false;
}

bool editor::resources::events::JumpBehaviour::writeParams(sol::table& params) {
    params[targetKey] = _target;
    return true;
}
