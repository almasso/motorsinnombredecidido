//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "OrCondition.h"

#include "../EventConditionFactory.h"

#define conditionAKey "conditionA"
#define conditionBKey "conditionB"

editor::resources::events::OrCondition::OrCondition() :
    _conditionA(nullptr),
    _conditionB(nullptr) {
}

editor::resources::events::OrCondition::~OrCondition() {
    delete _conditionA;
    delete _conditionB;
}

bool editor::resources::events::OrCondition::read(sol::table const& params) {
    sol::optional<sol::table> conditionA = params.get<sol::optional<sol::table>>(conditionAKey);
    if (!conditionA.has_value())
        return false;
    _conditionA = EventConditionFactory::Create(conditionA.value());
    if (_conditionA == nullptr)
        return false;

    sol::optional<sol::table> conditionB = params.get<sol::optional<sol::table>>(conditionBKey);
    if (!conditionB.has_value())
        return false;
    _conditionB = EventConditionFactory::Create(conditionB.value());
    if (_conditionB == nullptr)
        return false;

    return true;
}

bool editor::resources::events::OrCondition::writeToEngine(sol::table& condition) {
    return true;
}

bool editor::resources::events::OrCondition::writeParams(sol::table& params) {
    auto& l = io::LuaManager::GetInstance().getState();
    sol::table conditionA = l.create_table();
    if (!_conditionA->write(conditionA))
        return false;
    params[conditionAKey] = conditionA;

    sol::table conditionB = l.create_table();
    if (!_conditionB->write(conditionB))
        return false;
    params[conditionBKey] = conditionB;

    return true;
}
