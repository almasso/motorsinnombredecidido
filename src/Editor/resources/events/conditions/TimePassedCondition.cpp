//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "TimePassedCondition.h"

#define secondsKey "seconds"

editor::resources::events::TimePassedCondition::TimePassedCondition() :
    _seconds(0.0f) {
}

editor::resources::events::TimePassedCondition::~TimePassedCondition() = default;

bool editor::resources::events::TimePassedCondition::read(sol::table const& params) {
    _seconds = params.get_or<float, std::string, float>(secondsKey, -1.0f);
    return _seconds >= 0.0f;
}

bool editor::resources::events::TimePassedCondition::render() {
    return false;
}

bool editor::resources::events::TimePassedCondition::writeParamsToEngine(sol::table& params) {
    params[secondsKey] = _seconds;
    return true;
}

bool editor::resources::events::TimePassedCondition::writeParams(sol::table& params) {
    params[secondsKey] = _seconds;
    return true;
}
