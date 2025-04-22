//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "NotCondition.h"

#include <imgui.h>
#include <io/LocalizationManager.h>
#include <render/RenderManager.h>

#include "../EventConditionFactory.h"

#define conditionKey "condition"

editor::resources::events::NotCondition::NotCondition() :
    _condition(EventConditionFactory::Create("Always")) {
}

editor::resources::events::NotCondition::~NotCondition() {
    delete _condition;
}

bool editor::resources::events::NotCondition::read(sol::table const& params) {
    sol::optional<sol::table> condition = params.get<sol::optional<sol::table>>(conditionKey);
    if (!condition.has_value())
        return false;
    delete _condition;
    _condition = EventConditionFactory::Create(condition.value());
    if (_condition == nullptr)
        return false;
    return true;
}

bool editor::resources::events::NotCondition::render() {
    ImGui::BeginChild((std::string("##notCondition") + std::to_string(reinterpret_cast<long long>(this))).c_str(), ImVec2(0, 0), true | ImGuiChildFlags_AutoResizeY);
    bool edited = renderConditionSelector(_condition);
    edited = _condition->render() || edited;
    ImGui::EndChild();
    return edited;
}

bool editor::resources::events::NotCondition::writeParamsToEngine(sol::table& params) {
    sol::table condition = io::LuaManager::GetInstance().getState().create_table();
    if (!_condition->writeToEngine(condition))
        return false;
    params[conditionKey] = condition;
    return true;
}

bool editor::resources::events::NotCondition::writeParams(sol::table& params) {
    sol::table condition = io::LuaManager::GetInstance().getState().create_table();
    if (_condition && !_condition->write(condition))
        return false;
    params[conditionKey] = condition;
    return true;
}

bool editor::resources::events::NotCondition::renderConditionSelector(EventCondition*& condition) {
    bool edited = false;
    std::string conditionID(condition->getID());
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if (!ImGui::BeginCombo((std::string("###andConditionSelector") + std::to_string(reinterpret_cast<long long>(condition))).c_str(),
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
