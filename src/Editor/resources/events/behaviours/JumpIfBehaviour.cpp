//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "JumpIfBehaviour.h"

#include <imgui.h>
#include <io/LocalizationManager.h>
#include <resources/events/Event.h>

#include "../EventCondition.h"
#include "../EventConditionFactory.h"

#define targetKey "target"
#define conditionKey "condition"

editor::resources::events::JumpIfBehaviour::JumpIfBehaviour(Event* event) :
    EventBehaviourTemplate(event),
    _target(-1),
    _condition(EventConditionFactory::Create("OnStart")) {
}

editor::resources::events::JumpIfBehaviour::~JumpIfBehaviour() {
    delete _condition;
}

bool editor::resources::events::JumpIfBehaviour::read(sol::table const& params) {
    sol::optional<int> target = params.get<sol::optional<int>>(targetKey);
    if (!target.has_value())
        return false;
    _target = target.value();
    sol::optional<sol::table> condition = params.get<sol::optional<sol::table>>(conditionKey);
    if (!condition.has_value())
        return false;
    delete _condition;
    _condition = EventConditionFactory::Create(condition.value());
    return _condition != nullptr;
}

bool editor::resources::events::JumpIfBehaviour::writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) {
    return true;
}

bool editor::resources::events::JumpIfBehaviour::render() {
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition").c_str());
    bool edited = renderConditionSelector(_condition);
    edited = _condition->render() || edited;
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.JumpBehaviour.target").c_str());
    edited = renderBehaviourSelector() || edited;
    return edited;
}

bool editor::resources::events::JumpIfBehaviour::writeParams(sol::table& params) {
    params[targetKey] = _target;
    sol::table condition = io::LuaManager::GetInstance().getState().create_table();
    if (!_condition->write(condition))
        return false;
    params[conditionKey] = condition;
    return true;
}

bool editor::resources::events::JumpIfBehaviour::renderConditionSelector(EventCondition*& condition) {
    bool edited = false;
    std::string conditionID(condition->getID());
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if (!ImGui::BeginCombo((std::string("###jumpIfConditionSelector") + std::to_string(reinterpret_cast<long long>(condition))).c_str(),
        io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition." + conditionID).c_str()))
        return false;

    auto const& conditions = resources::events::EventConditionFactory::GetKeys();
    for (auto const& conditionName : conditions) {
        bool isSelected = (conditionName == conditionID);
        if (ImGui::Selectable(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition." + conditionName).c_str(), isSelected)) {
            if (!isSelected) {
                delete condition;
                condition = EventConditionFactory::Create(conditionName);
                edited = true;
            }
        }
    }
    ImGui::EndCombo();
    return edited;
}


bool editor::resources::events::JumpIfBehaviour::renderBehaviourSelector() {
    auto const& behaviours = _event->getBehaviours();
    std::string selectedBehaviour;
    if (_target < 0 || _target >= behaviours.size())
        selectedBehaviour = "invalid";
    else {
        auto selected = behaviours.begin();
        for (int i = 0; i < _target; ++i, ++selected);
        selectedBehaviour = (*selected)->getID();
    }

    std::string previewValue;
    if (_target >= 0) {
        previewValue += std::to_string(_target);
        previewValue += " - ";
    }
    previewValue += io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours." + selectedBehaviour);

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if (!ImGui::BeginCombo((std::string("###jumpIfBehaviourSelector") + std::to_string(reinterpret_cast<long long>(this))).c_str(),
                           previewValue.c_str()))
        return false;

    bool edited = false;
    int i = 0;
    for (auto const& behaviour : behaviours) {
        bool isSelected = (i == _target);
        std::string label(std::to_string(i));
        label += " - ";
        label += io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours." + std::string(behaviour->getID()));
        if (ImGui::Selectable(label.c_str(), isSelected)) {
            if (!isSelected) {
                _target = i;
                edited = true;
            }
        }
        ++i;
    }
    ImGui::EndCombo();
    return edited;
}