//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "CollidesWithPlayerCondition.h"

#include <imgui.h>
#include <common/Project.h>
#include <io/LocalizationManager.h>
#include <resources/Map.h>
#include <resources/events/Event.h>

#define objectAKey "objectA"
#define objectBKey "objectB"

editor::resources::events::CollidesWithPlayerCondition::CollidesWithPlayerCondition(Event* event) :
    EventConditionTemplate(event) {
}

editor::resources::events::CollidesWithPlayerCondition::~CollidesWithPlayerCondition() = default;

bool editor::resources::events::CollidesWithPlayerCondition::read(sol::table const& params) {
    return true;
}

bool editor::resources::events::CollidesWithPlayerCondition::render() {
    return false;
}

bool editor::resources::events::CollidesWithPlayerCondition::writeParamsToEngine(std::ostream& condition, EventBuildDependencies& dependencies, Object const* container) {
    sol::table colliderParams = io::LuaManager::GetInstance().getState().create_table();
    sol::table sizeTable = io::LuaManager::GetInstance().getState().create_table();
    const int* dims = _event->getProject()->getDimensions();
    sizeTable["x"] = dims[0];
    sizeTable["y"] = dims[1];
    colliderParams["size"] = sizeTable;
    dependencies.componentDependencies.insert({"Collider", colliderParams});
    return true;
}

bool editor::resources::events::CollidesWithPlayerCondition::writeParams(sol::table& params) {
    return true;
}
