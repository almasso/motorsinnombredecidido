//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "InteractionCondition.h"

#include <common/Project.h>
#include <resources/events/Event.h>

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

bool editor::resources::events::InteractionCondition::writeParamsToEngine(std::ostream& condition, EventBuildDependencies& dependencies, Object const* container) {
    std::string handler(std::to_string(reinterpret_cast<long long>(this)) + std::to_string(reinterpret_cast<long long>(container)));
    condition << "interactionArea = \"" << handler << "\"";
    writeDependencies(dependencies, handler);
    return true;
}

bool editor::resources::events::InteractionCondition::writeParams(sol::table& params) {
    params.add(0);
    return true;
}

void editor::resources::events::InteractionCondition::writeTransform(sol::state& lua, const int* dims, sol::table& transformParams) {
    sol::table posTable = lua.create_table();
    posTable.add(-dims[0]);
    posTable.add(-dims[1]);
}

void editor::resources::events::InteractionCondition::writeCollider(sol::state& lua, const int* dims, sol::table& colliderParams) {
    sol::table sizeTable = lua.create_table();
    sizeTable.add(dims[0] * 3);
    sizeTable.add(dims[1] * 3);
    colliderParams["size"] = sizeTable;
}

void editor::resources::events::InteractionCondition::writeDependencies(EventBuildDependencies& dependencies, std::string const& handler) {
    EventBuildDependencies::ComponentsMap components;
    auto& lua = io::LuaManager::GetInstance().getState();
    auto dims = _event->getProject()->getDimensions();

    sol::table colliderParams = lua.create_table();
    writeCollider(lua, dims, colliderParams);
    components.insert({"Collider", colliderParams});

    sol::table transformParams = lua.create_table();
    writeTransform(lua, dims, transformParams);
    components.insert({"Transform", transformParams});

    dependencies.childrenDependencies.push_back({handler, components});
}