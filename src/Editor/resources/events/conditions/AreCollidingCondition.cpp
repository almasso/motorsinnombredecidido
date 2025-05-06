//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "AreCollidingCondition.h"

#include <imgui.h>
#include <common/Project.h>
#include <io/LocalizationManager.h>
#include <resources/Map.h>
#include <resources/events/Event.h>

#define objectAKey "objectA"
#define objectBKey "objectB"

editor::resources::events::AreCollidingCondition::AreCollidingCondition(Event* event) :
    EventConditionTemplate(event),
    _objectA(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition.AreColliding.objectA")),
    _objectB(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition.AreColliding.objectB")) {
}

editor::resources::events::AreCollidingCondition::~AreCollidingCondition() = default;

bool editor::resources::events::AreCollidingCondition::read(sol::table const& params) {
    sol::optional<std::string> objectA = params.get<sol::optional<std::string>>(objectAKey);
    if (!objectA.has_value())
        return false;
    _objectA = objectA.value();

    sol::optional<std::string> objectB = params.get<sol::optional<std::string>>(objectBKey);
    if (!objectB.has_value())
        return false;
    _objectB = objectB.value();

    return true;
}

bool editor::resources::events::AreCollidingCondition::render() {
    bool edited = renderObjectSelector(_objectA, 0);
    edited = renderObjectSelector(_objectB, 1) || edited;
    return edited;
}

bool editor::resources::events::AreCollidingCondition::writeParamsToEngine(std::ostream& condition, EventBuildDependencies& dependencies, Object const* container) {
    condition << "colliderA" << " = \"" << _objectA << "\",\n";
    condition << "colliderB" << " = \"" << _objectB << "\"";
    return true;
}

bool editor::resources::events::AreCollidingCondition::writeParams(sol::table& params) {
    params[objectAKey] = _objectA;
    params[objectBKey] = _objectB;
    return true;
}

bool editor::resources::events::AreCollidingCondition::renderObjectSelector(std::string& objectSelected, int i) {
    bool edited = false;

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if (!ImGui::BeginCombo((std::string("##areCollidingObjectSelector") + std::to_string(i) + std::to_string(reinterpret_cast<long long>(this))).c_str(),
        objectSelected.c_str()))
        return false;

    auto const& maps = _event->getProject()->getMaps();
    for (auto const& [mapName, map] : maps) {
        auto const& objects = map->getObjects();
        for (auto const& [pos, object] : objects) {
            std::string objectName = getObjectName(mapName, pos);
            bool isSelected = (objectName == objectSelected);
            if (ImGui::Selectable((objectName + "##areCollidingObject" + std::to_string(reinterpret_cast<long long>(this))).c_str(), isSelected)) {
                if (!isSelected) {
                    objectSelected = objectName;
                    edited = true;
                }
            }
        }
    }
    ImGui::EndCombo();
    return edited;
}

std::string editor::resources::events::AreCollidingCondition::getObjectName(std::string const& map, int object) {
    return ("object_" + map + "_" + std::to_string(object));
}
