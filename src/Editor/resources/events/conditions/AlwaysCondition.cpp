//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "AlwaysCondition.h"

editor::resources::events::AlwaysCondition::AlwaysCondition() = default;

editor::resources::events::AlwaysCondition::~AlwaysCondition() = default;

bool editor::resources::events::AlwaysCondition::read(sol::table const& params) {
    return true;
}

bool editor::resources::events::AlwaysCondition::writeToEngine(sol::table& condition) {
    return true;
}

bool editor::resources::events::AlwaysCondition::writeParams(sol::table& params) {
    params.add(0);
    return true;
}
