//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "ValueEqualsCondition.h"

#define variableKey "variable"
#define equalsKey "equals"

editor::resources::events::ValueEqualsCondition::ValueEqualsCondition() :
    _variable(),
    _equals(sol::lua_nil) {
}

editor::resources::events::ValueEqualsCondition::~ValueEqualsCondition() = default;

bool editor::resources::events::ValueEqualsCondition::read(sol::table const& params) {
    _variable = params.get_or<std::string>(variableKey, "");
    if (_variable.empty())
        return false;

    sol::optional<sol::lua_value> equals = params.get<sol::optional<sol::lua_value>>(equalsKey);
    if (!equals.has_value())
        return false;
    _equals = equals.value();

    return true;
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
