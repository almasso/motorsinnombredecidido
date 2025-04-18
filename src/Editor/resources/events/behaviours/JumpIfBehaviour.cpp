//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "JumpIfBehaviour.h"

#include "../EventCondition.h"
#include "../EventConditionFactory.h"

#define targetKey "target"
#define conditionKey "condition"

editor::resources::events::JumpIfBehaviour::JumpIfBehaviour() :
    _target(-1),
    _condition(nullptr) {
}

editor::resources::events::JumpIfBehaviour::~JumpIfBehaviour() {
    delete _condition;
}

bool editor::resources::events::JumpIfBehaviour::read(sol::table const& params) {
    sol::optional<int> target = params.get<sol::optional<int>>(targetKey);
    if (!target.has_value())
        return false;
    _target = target.value();
    sol::optional<sol::table> condition = params.get<sol::optional<sol::table>>(conditionKey);
    if (!condition.has_value())
        return false;
    _condition = EventConditionFactory::Create(condition.value());
    return _condition != nullptr;
}

bool editor::resources::events::JumpIfBehaviour::writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) {
    return true;
}

bool editor::resources::events::JumpIfBehaviour::render() {
    return false;
}

bool editor::resources::events::JumpIfBehaviour::writeParams(sol::table& params) {
    params[targetKey] = _target;
    sol::table condition = io::LuaManager::GetInstance().getState().create_table();
    if (!_condition->write(condition))
        return false;
    params[conditionKey] = condition;
    return true;
}
