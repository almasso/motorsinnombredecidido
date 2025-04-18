//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "ChoiceBehaviour.h"

#define variableKey "variable"
#define optionsKey "options"

editor::resources::events::ChoiceBehaviour::ChoiceBehaviour() :
    _variable(),
    _options() {
}

editor::resources::events::ChoiceBehaviour::~ChoiceBehaviour() = default;

bool editor::resources::events::ChoiceBehaviour::read(sol::table const& params) {
    _variable = params.get_or<std::string>(variableKey, "");
    if (_variable.empty())
        return false;
    sol::optional<sol::table> options = params.get<sol::optional<sol::table>>(optionsKey);
    if (!options.has_value())
        return false;
    for (auto& [key, value] : options.value()) {
        if (!key.is<std::string>() || !value.is<sol::lua_value>())
            return false;
        _options.push_back({key.as<std::string>(), value.as<sol::lua_value>()});
    }
    return true;
}

bool editor::resources::events::ChoiceBehaviour::writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) {
    return true;
}

bool editor::resources::events::ChoiceBehaviour::render() {
    return false;
}

bool editor::resources::events::ChoiceBehaviour::writeParams(sol::table& params) {
    params[variableKey] = _variable;
    auto& l = io::LuaManager::GetInstance().getState();
    sol::table options = l.create_table();
    for (auto& [key, value] : _options) {
        options[key] = value;
    }
    params[optionsKey] = options;
    return true;
}
