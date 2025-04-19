//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "ValueEqualsCondition.h"

#define variableKey "variable"
#define equalsKey "equals"

editor::resources::events::ValueEqualsCondition::ValueEqualsCondition() :
    _variable(),
    _equals(0) {
}

editor::resources::events::ValueEqualsCondition::~ValueEqualsCondition() = default;

bool editor::resources::events::ValueEqualsCondition::read(sol::table const& params) {
    sol::optional<std::string> variable = params.get<sol::optional<std::string>>(variableKey);
    if (!variable.has_value())
        return false;
    _variable = variable.value();

    sol::optional<sol::lua_value> equals = params.get<sol::optional<sol::lua_value>>(equalsKey);
    if (!equals.has_value())
        return false;
    _equals = equals.value();

    return true;
}

bool editor::resources::events::ValueEqualsCondition::render() {
    return false;
}

bool editor::resources::events::ValueEqualsCondition::writeParamsToEngine(sol::table& params) {
    params[variableKey] = _variable;
    params[equalsKey] = _equals;
    return true;
}

bool editor::resources::events::ValueEqualsCondition::writeParams(sol::table& params) {
    params[variableKey] = _variable;
    params[equalsKey] = _equals;
    return true;
}
