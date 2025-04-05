//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Event.h"

bool editor::resources::Event::read(std::string const& name, sol::table const& eventTable) {
    return true;
}

bool editor::resources::Event::write(sol::table& eventTable) {

    return true;
}

std::string const& editor::resources::Event::getName() const {
    return _name;
}
