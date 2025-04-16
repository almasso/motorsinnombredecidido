//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "EventConditionFactory.h"

#include "conditions/OnStartCondition.h"

#include "EventCondition.h"

std::unordered_map <std::string, std::function<editor::resources::events::EventCondition*()>> editor::resources::events::EventConditionFactory::_conditions;
bool editor::resources::events::EventConditionFactory::_initialized = false;

editor::resources::events::EventCondition* editor::resources::events::EventConditionFactory::Create(std::string const& id) {
    if (!_initialized)
        Init();

    auto it = _conditions.find(id);
    if (it == _conditions.end())
        return nullptr;
    return it->second();
}

editor::resources::events::EventCondition* editor::resources::events::EventConditionFactory::Create(sol::table const& condition) {
    std::string id = condition.get_or<std::string>(typeKey, "");
    if (id.empty())
        return nullptr;

    sol::optional<sol::table> params = condition.get<sol::optional<sol::table>>(paramsKey);
    if (!params.has_value())
        return nullptr;

    auto instance = Create(id);
    if (!instance)
        return nullptr;

    if (instance->read(*params))
        return instance;

    delete instance;
    return nullptr;
}

void editor::resources::events::EventConditionFactory::Init() {
    _initialized = true;
    RegisterCondition<OnStartCondition>();
}
