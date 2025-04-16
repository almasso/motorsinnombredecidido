//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "ModifyLocalVariableBehaviour.h"

#define variableKey "variable"
#define newValueKey "newValue"

editor::resources::events::ModifyLocalVariableBehaviour::ModifyLocalVariableBehaviour() :
    _variable(),
    _newValue(sol::lua_nil) {
}

editor::resources::events::ModifyLocalVariableBehaviour::~ModifyLocalVariableBehaviour() = default;

bool editor::resources::events::ModifyLocalVariableBehaviour::read(sol::table const& params) {
    _variable = params.get_or<std::string>(variableKey, "");
    if (_variable.empty())
        return false;
    sol::optional<sol::lua_value> newValue = params.get<sol::optional<sol::lua_value>>(newValueKey);
    if (!newValue.has_value())
        return false;
    _newValue = newValue.value();
    return true;
}

bool editor::resources::events::ModifyLocalVariableBehaviour::writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) {
    return true;
}

bool editor::resources::events::ModifyLocalVariableBehaviour::writeParams(sol::table& params) {
    params[variableKey] = _variable;
    params[newValueKey] = _newValue;
    return true;
}

