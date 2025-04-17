//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "NotCondition.h"

#include "../EventConditionFactory.h"

#define conditionKey "condition"

editor::resources::events::NotCondition::NotCondition() :
    _condition(nullptr) {
}

editor::resources::events::NotCondition::~NotCondition() {
    delete _condition;
}

bool editor::resources::events::NotCondition::read(sol::table const& params) {
    sol::optional<sol::table> condition = params.get<sol::optional<sol::table>>(conditionKey);
    if (!condition.has_value())
        return false;
    _condition = EventConditionFactory::Create(condition.value());
    if (_condition == nullptr)
        return false;
    return true;
}

bool editor::resources::events::NotCondition::writeParamsToEngine(sol::table& params) {
    sol::table condition = io::LuaManager::GetInstance().getState().create_table();
    if (!_condition->writeToEngine(condition))
        return false;
    params[conditionKey] = condition;
    return true;
}

bool editor::resources::events::NotCondition::writeParams(sol::table& params) {
    sol::table condition = io::LuaManager::GetInstance().getState().create_table();
    if (!_condition->write(condition))
        return false;
    params[conditionKey] = condition;
    return true;
}
