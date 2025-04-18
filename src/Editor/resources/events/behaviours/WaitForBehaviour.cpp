//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "WaitForBehaviour.h"

#include "../EventCondition.h"
#include "../EventConditionFactory.h"

#define conditionKey "condition"

editor::resources::events::WaitForBehaviour::WaitForBehaviour() :
    _condition(nullptr) {
}

editor::resources::events::WaitForBehaviour::~WaitForBehaviour() {
    delete _condition;
}

bool editor::resources::events::WaitForBehaviour::read(sol::table const& params) {
    sol::optional<sol::table> condition = params.get<sol::optional<sol::table>>(conditionKey);
    if (!condition.has_value())
        return false;
    _condition = EventConditionFactory::Create(condition.value());
    return _condition != nullptr;
}

bool editor::resources::events::WaitForBehaviour::writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) {
    return true;
}

bool editor::resources::events::WaitForBehaviour::render() {
    return false;
}

bool editor::resources::events::WaitForBehaviour::writeParams(sol::table& params) {
    sol::table condition = io::LuaManager::GetInstance().getState().create_table();
    if (!_condition->write(condition))
        return false;
    params[conditionKey] = condition;
    return true;
}
