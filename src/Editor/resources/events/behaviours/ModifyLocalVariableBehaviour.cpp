//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "ModifyLocalVariableBehaviour.h"

#define variableKey "variable"
#define newValueKey "newValue"

editor::resources::events::ModifyLocalVariableBehaviour::ModifyLocalVariableBehaviour(Event* event) :
    EventBehaviourTemplate(event),
    _variable(),
    _newValue(0) {
}

editor::resources::events::ModifyLocalVariableBehaviour::~ModifyLocalVariableBehaviour() = default;

bool editor::resources::events::ModifyLocalVariableBehaviour::read(sol::table const& params) {
    sol::optional<std::string> variable = params.get<sol::optional<std::string>>(variableKey);
    if (!variable.has_value())
        return false;
    _variable = variable.value();
    sol::optional<sol::lua_value> newValue = params.get<sol::optional<sol::lua_value>>(newValueKey);
    if (!newValue.has_value())
        return false;
    _newValue = newValue.value();
    return true;
}

bool editor::resources::events::ModifyLocalVariableBehaviour::writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) {
    return true;
}

bool editor::resources::events::ModifyLocalVariableBehaviour::render() {
    return false;
}

bool editor::resources::events::ModifyLocalVariableBehaviour::writeParams(sol::table& params) {
    params[variableKey] = _variable;
    params[newValueKey] = _newValue;
    return true;
}

