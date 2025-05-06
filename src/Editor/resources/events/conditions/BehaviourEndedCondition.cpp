//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "BehaviourEndedCondition.h"

#include <common/Project.h>
#include <io/LocalizationManager.h>
#include <resources/Map.h>
#include <resources/Object.h>
#include <resources/events/EventBehaviour.h>
#include "../Event.h"

#define entityKey "entity"
#define eventKey "event"
#define behaviourKey "behaviour"

editor::resources::events::BehaviourEndedCondition::BehaviourEndedCondition(Event* event) :
    EventConditionTemplate(event),
    _conditionObject(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition.BehaviourEnded.object")),
    _conditionEvent(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition.BehaviourEnded.event")),
    _conditionBehaviour(-1) {
}

editor::resources::events::BehaviourEndedCondition::~BehaviourEndedCondition() = default;

bool editor::resources::events::BehaviourEndedCondition::read(sol::table const& params) {
    sol::optional<std::string> entity = params.get<sol::optional<std::string>>(entityKey);
    if (!entity.has_value())
        return false;
    _conditionObject = entity.value();

    sol::optional<std::string> event = params.get<sol::optional<std::string>>(eventKey);
    if (!event.has_value())
        return false;
    _conditionEvent = event.value();

    _conditionBehaviour = params.get_or<int, std::string, int>(behaviourKey, -2);
    if (_conditionBehaviour == -2)
        return false;

    return true;
}

bool editor::resources::events::BehaviourEndedCondition::render() {
    Object* selectedObject = nullptr;
    Event* selectedEvent = nullptr;
    bool edited = renderObjectSelector(selectedObject);
    edited = renderEventSelector(selectedObject, selectedEvent) || edited;
    edited = renderBehaviourSelector(selectedEvent) || edited;
    return edited;
}

bool editor::resources::events::BehaviourEndedCondition::writeParamsToEngine(std::ostream& condition, EventBuildDependencies& dependencies) {
    condition << entityKey << " = \"" << _conditionObject << "\",\n";
    condition << eventKey << " = \"" << _conditionEvent << "\",\n";
    condition << behaviourKey << " = " << _conditionBehaviour << "\n";
    return true;
}

bool editor::resources::events::BehaviourEndedCondition::writeParams(sol::table& params) {
    params[entityKey] = _conditionObject;
    params[eventKey] = _conditionEvent;
    params[behaviourKey] = _conditionBehaviour;
    return true;
}

bool editor::resources::events::BehaviourEndedCondition::renderObjectSelector(Object*& selectedObject) {
    bool edited = false;

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    bool render = ImGui::BeginCombo((std::string("##behaviourEndedObjectSelector") + std::to_string(reinterpret_cast<long long>(this))).c_str(),
        _conditionObject.c_str());

    auto const& maps = _event->getProject()->getMaps();
    for (auto const& [mapName, map] : maps) {
        auto const& objects = map->getObjects();
        for (auto const& [pos, object] : objects) {
            std::string objectName = getObjectName(mapName, pos);
            bool isSelected = (objectName == _conditionObject);
            if (isSelected)
                selectedObject = object;
            if (render && ImGui::Selectable((objectName + "##behaviourEndedObject" + std::to_string(reinterpret_cast<long long>(this))).c_str(), isSelected)) {
                if (!isSelected) {
                    _conditionObject = objectName;
                    selectedObject = object;
                    edited = true;
                }
            }
        }
    }
    if (render)
        ImGui::EndCombo();
    return edited;
}

bool editor::resources::events::BehaviourEndedCondition::renderEventSelector(Object* selectedObject, Event*& selectedEvent) {
    if (selectedObject == nullptr)
        return false;
    bool edited = false;

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    bool render = ImGui::BeginCombo((std::string("##behaviourEndedEventSelector") + std::to_string(reinterpret_cast<long long>(this))).c_str(),
        _conditionEvent.c_str());

    auto const& events = selectedObject->getEvents();
    for (auto const& event : events) {
        bool isSelected = (event->getName() == _conditionEvent);
        if (isSelected)
            selectedEvent = event;
        if (render && ImGui::Selectable((event->getName() + "##behaviourEndedEvent" + std::to_string(reinterpret_cast<long long>(this))).c_str(), isSelected)) {
            if (!isSelected) {
                _conditionEvent = event->getName();
                selectedEvent = event;
                edited = true;
            }
        }
    }
    if (render)
        ImGui::EndCombo();
    return edited;
}

bool editor::resources::events::BehaviourEndedCondition::renderBehaviourSelector(Event* selectedEvent) {
    if (selectedEvent == nullptr)
        return false;

    bool edited = false;

    std::string selectedBehaviourID;
    if (_conditionBehaviour >= 0 && _conditionBehaviour < selectedEvent->getBehaviours().size()) {
        auto behaviourIt = selectedEvent->getBehaviours().begin();
        for (int i = 0; i < _conditionBehaviour; ++i) ++behaviourIt;
        selectedBehaviourID = std::to_string(_conditionBehaviour) + " - " + (*behaviourIt)->getID();
    }
    else selectedBehaviourID = io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition.BehaviourEnded.behaviour");

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if (!ImGui::BeginCombo((std::string("##behaviourEndedBehaviourSelector") + std::to_string(reinterpret_cast<long long>(this))).c_str(),
        selectedBehaviourID.c_str()))
        return false;

    int i = 0;
    auto const& behaviours = selectedEvent->getBehaviours();
    for (auto const& behaviour : behaviours) {
        bool isSelected = (i == _conditionBehaviour);
        if (ImGui::Selectable((std::to_string(i) + " - " + behaviour->getID() + "##behaviourEndedBehaviour" + std::to_string(reinterpret_cast<long long>(this))).c_str(), isSelected)) {
            if (!isSelected) {
                _conditionBehaviour = i;
                edited = true;
            }
        }
        ++i;
    }
    ImGui::EndCombo();
    return edited;
}

std::string editor::resources::events::BehaviourEndedCondition::getObjectName(std::string const& map, int object) {
    return ("object_" + map + "_" + std::to_string(object));
}
