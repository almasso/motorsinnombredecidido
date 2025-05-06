//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "InteractionCondition.h"

editor::resources::events::InteractionCondition::InteractionCondition(Event* event) :
    EventConditionTemplate(event) {
}

editor::resources::events::InteractionCondition::~InteractionCondition() = default;

bool editor::resources::events::InteractionCondition::read(sol::table const& params) {
    return true;
}

bool editor::resources::events::InteractionCondition::render() {
    return false;
}

bool editor::resources::events::InteractionCondition::writeParamsToEngine(std::ostream& condition, EventBuildDependencies& dependencies) {
    return true;
}

bool editor::resources::events::InteractionCondition::writeParams(sol::table& params) {
    params.add(0);
    return true;
}