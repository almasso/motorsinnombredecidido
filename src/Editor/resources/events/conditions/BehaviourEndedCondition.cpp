//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "BehaviourEndedCondition.h"

#define entityKey "entity"
#define eventKey "event"
#define behaviourKey "behaviour"

editor::resources::events::BehaviourEndedCondition::BehaviourEndedCondition() :
    _entity(),
    _event(),
    _behaviour(-1) {
}

editor::resources::events::BehaviourEndedCondition::~BehaviourEndedCondition() = default;

bool editor::resources::events::BehaviourEndedCondition::read(sol::table const& params) {
    sol::optional<std::string> entity = params.get<sol::optional<std::string>>(entityKey);
    if (!entity.has_value())
        return false;
    _entity = entity.value();

    sol::optional<std::string> event = params.get<sol::optional<std::string>>(eventKey);
    if (!event.has_value())
        return false;
    _event = event.value();

    _behaviour = params.get_or<int, std::string, int>(behaviourKey, -2);
    if (_behaviour == -2)
        return false;

    return true;
}

bool editor::resources::events::BehaviourEndedCondition::render() {
    return false;
}

bool editor::resources::events::BehaviourEndedCondition::writeParamsToEngine(sol::table& params) {
    params[entityKey] = _entity;
    params[eventKey] = _event;
    params[behaviourKey] = _behaviour;
    return true;
}

bool editor::resources::events::BehaviourEndedCondition::writeParams(sol::table& params) {
    params[entityKey] = _entity;
    params[eventKey] = _event;
    params[behaviourKey] = _behaviour;
    return true;
}
