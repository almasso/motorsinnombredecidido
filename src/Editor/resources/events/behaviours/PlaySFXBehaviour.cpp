//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "PlaySFXBehaviour.h"

#define sfxKey "sfx"

editor::resources::events::PlaySFXBehaviour::PlaySFXBehaviour() :
    _sfxHandler() {
}

editor::resources::events::PlaySFXBehaviour::~PlaySFXBehaviour() = default;

bool editor::resources::events::PlaySFXBehaviour::read(sol::table const& params) {
    _sfxHandler = params.get_or<std::string>(sfxKey, "");
    return !_sfxHandler.empty();
}

bool editor::resources::events::PlaySFXBehaviour::writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) {
    return true;
}

bool editor::resources::events::PlaySFXBehaviour::render() {
    return false;
}

bool editor::resources::events::PlaySFXBehaviour::writeParams(sol::table& params) {
    params[sfxKey] = _sfxHandler;
    return true;
}
