//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "OnStartCondition.h"

editor::resources::events::OnStartCondition::OnStartCondition(Event* event) :
    EventConditionTemplate(event) {
}

editor::resources::events::OnStartCondition::~OnStartCondition() {
}

bool editor::resources::events::OnStartCondition::read(sol::table const& params) {
    return true;
}

bool editor::resources::events::OnStartCondition::render() {
    return false;
}

bool editor::resources::events::OnStartCondition::writeParamsToEngine(std::ostream& condition, EventBuildDependencies& dependencies, Object const* container) {
    return true;
}

bool editor::resources::events::OnStartCondition::writeParams(sol::table& params) {
    params.add(0);
    return true;
}
