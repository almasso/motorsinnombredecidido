//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "DialogueBehaviour.h"

#define textKey "text"

editor::resources::events::DialogueBehaviour::DialogueBehaviour() :
    _text() {
}

editor::resources::events::DialogueBehaviour::~DialogueBehaviour() = default;

bool editor::resources::events::DialogueBehaviour::read(sol::table const& params) {
    _text = params.get_or<std::string>(textKey, "");
    return !_text.empty();
}

bool editor::resources::events::DialogueBehaviour::writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) {
    return true;
}

bool editor::resources::events::DialogueBehaviour::render() {
    return false;
}

bool editor::resources::events::DialogueBehaviour::writeParams(sol::table& params) {
    params[textKey] = _text;
    return true;
}
