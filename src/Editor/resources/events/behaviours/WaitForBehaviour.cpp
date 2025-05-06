//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "WaitForBehaviour.h"

#include <imgui.h>
#include <io/LocalizationManager.h>

#include "../EventCondition.h"
#include "../EventConditionFactory.h"

#define conditionKey "condition"

editor::resources::events::WaitForBehaviour::WaitForBehaviour(Event* event) :
    EventBehaviourTemplate(event),
    _condition(EventConditionFactory::Create("OnStart", event)) {
}

editor::resources::events::WaitForBehaviour::~WaitForBehaviour() {
    delete _condition;
}

bool editor::resources::events::WaitForBehaviour::read(sol::table const& params) {
    sol::optional<sol::table> condition = params.get<sol::optional<sol::table>>(conditionKey);
    if (!condition.has_value())
        return false;
    delete _condition;
    _condition = EventConditionFactory::Create(condition.value(), _event);
    return _condition != nullptr;
}

bool editor::resources::events::WaitForBehaviour::writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies) {
    return true;
}

bool editor::resources::events::WaitForBehaviour::render() {
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition").c_str());
    bool edited = renderConditionSelector(_condition);
    edited = _condition->render() || edited;
    return edited;
}

bool editor::resources::events::WaitForBehaviour::writeParams(sol::table& params) {
    sol::table condition = io::LuaManager::GetInstance().getState().create_table();
    if (!_condition->write(condition))
        return false;
    params[conditionKey] = condition;
    return true;
}

bool editor::resources::events::WaitForBehaviour::renderConditionSelector(EventCondition*& condition) {
    bool edited = false;
    std::string conditionID(condition->getID());
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if (!ImGui::BeginCombo((std::string("###waitForConditionSelector") + std::to_string(reinterpret_cast<long long>(condition))).c_str(),
        io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition." + conditionID).c_str()))
        return false;

    auto const& conditions = resources::events::EventConditionFactory::GetKeys();
    for (auto const& conditionName : conditions) {
        bool isSelected = (conditionName == conditionID);
        if (ImGui::Selectable(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition." + conditionName).c_str(), isSelected)) {
            if (!isSelected) {
                delete condition;
                condition = EventConditionFactory::Create(conditionName, _event);
                edited = true;
            }
        }
    }
    ImGui::EndCombo();
    return edited;
}