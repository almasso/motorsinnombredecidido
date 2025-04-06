//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "OnStartCondition.h"

editor::resources::events::OnStartCondition::~OnStartCondition() {
}

bool editor::resources::events::OnStartCondition::read(sol::table const& condition) {
    return true;
}

bool editor::resources::events::OnStartCondition::write(sol::table& condition) {
    return true;
}

bool editor::resources::events::OnStartCondition::writeToEngine(sol::table& condition) {
    return true;
}
