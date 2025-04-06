//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "MoveBehaviour.h"

editor::resources::events::MoveBehaviour::~MoveBehaviour() {
}

bool editor::resources::events::MoveBehaviour::read(sol::table const& condition) {
    return true;
}

bool editor::resources::events::MoveBehaviour::write(sol::table& condition) {
    return true;
}

bool editor::resources::events::MoveBehaviour::writeToEngine(sol::table& condition) {
    return true;
}