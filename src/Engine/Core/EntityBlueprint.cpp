#include "EntityBlueprint.h"
/*
#include <sol/sol.hpp>

#include "ComponentData.h"

void EntityBlueprint::readChildren(sol::table const& childrenBlueprint) {
    for (auto const& [key,child] : childrenBlueprint) {
        if (child.is<sol::table>())
            _children.emplace_back(child.as<sol::table>());
    }
}

void EntityBlueprint::readComponents(sol::table const& componentBlueprints) {
    for (auto const& [id,component] : componentBlueprints) {
        if (!id.is<std::string>() || !component.is<sol::table>())
            continue;
        _components.push_back(new ComponentData(id.as<std::string>(), component.as<sol::table>()));
    }
}

void EntityBlueprint::read(sol::table const& blueprint) {
    sol::table childrenBlueprints = blueprint.get_or<sol::table>("children", nullptr);
    if (childrenBlueprints != nullptr)
        readChildren(childrenBlueprints);

    sol::table componentBlueprints = blueprint.get_or<sol::table>("components", nullptr);
    if (componentBlueprints != nullptr)
        readComponents(componentBlueprints);

    handler = blueprint.get_or<std::string>("handler", "");

    active = blueprint.get_or<bool, std::string, bool>("active", true);
}

void EntityBlueprint::clear() {
    _children.clear();
    for (auto const& component : _components) {
        delete component;
    }
    _components.clear();
    active = false;
    handler = "";
}

EntityBlueprint::EntityBlueprint() = default;

EntityBlueprint::~EntityBlueprint() {
    clear();
}

EntityBlueprint::EntityBlueprint(sol::table const& blueprint) {
    read(blueprint);
}
*/