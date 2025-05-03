//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "EventConditionFactory.h"

#include "EventCondition.h"

#include "conditions/AlwaysCondition.h"
#include "conditions/AndCondition.h"
#include "conditions/AreCollidingCondition.h"
#include "conditions/BehaviourEndedCondition.h"
#include "conditions/NotCondition.h"
#include "conditions/OnStartCondition.h"
#include "conditions/OrCondition.h"
#include "conditions/TimePassedCondition.h"
#include "conditions/ValueEqualsCondition.h"


std::unordered_map <std::string, std::function<editor::resources::events::EventCondition*(editor::resources::events::Event*)>> editor::resources::events::EventConditionFactory::_conditions;
std::set<std::string> editor::resources::events::EventConditionFactory::_conditionKeys;
bool editor::resources::events::EventConditionFactory::_initialized = false;

editor::resources::events::EventCondition* editor::resources::events::EventConditionFactory::Create(std::string const& id, Event* event) {
    if (!_initialized)
        Init();

    auto it = _conditions.find(id);
    if (it == _conditions.end())
        return nullptr;
    return it->second(event);
}

editor::resources::events::EventCondition* editor::resources::events::EventConditionFactory::Create(sol::table const& condition, Event* event) {
    std::string id = condition.get_or<std::string>(typeKey, "");
    if (id.empty())
        return nullptr;

    sol::optional<sol::table> params = condition.get<sol::optional<sol::table>>(paramsKey);
    if (!params.has_value())
        return nullptr;

    auto instance = Create(id, event);
    if (!instance)
        return nullptr;

    if (instance->read(*params))
        return instance;

    delete instance;
    return nullptr;
}

std::set<std::string> const& editor::resources::events::EventConditionFactory::GetKeys() {
    return _conditionKeys;
}

void editor::resources::events::EventConditionFactory::Init() {
    _initialized = true;
    RegisterCondition<AlwaysCondition>();
    RegisterCondition<AndCondition>();
    RegisterCondition<AreCollidingCondition>();
    RegisterCondition<BehaviourEndedCondition>();
    RegisterCondition<NotCondition>();
    RegisterCondition<OnStartCondition>();
    RegisterCondition<OrCondition>();
    RegisterCondition<TimePassedCondition>();
    RegisterCondition<ValueEqualsCondition>();
}
