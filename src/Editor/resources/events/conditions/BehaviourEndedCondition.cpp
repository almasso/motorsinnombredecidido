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
    _entity = params.get_or<std::string>(entityKey, "");
    if (_entity.empty())
        return false;

    _event = params.get_or<std::string>(eventKey, "");
    if (_event.empty())
        return false;

    _behaviour = params.get_or<int, std::string, int>(behaviourKey, -1);
    if (_behaviour == -1)
        return false;

    return true;
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
