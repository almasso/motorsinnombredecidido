//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "OrCondition.h"

#include <imgui.h>
#include <io/LocalizationManager.h>
#include <render/RenderManager.h>

#include "../EventConditionFactory.h"

#define conditionAKey "conditionA"
#define conditionBKey "conditionB"

editor::resources::events::OrCondition::OrCondition() :
    _conditionA(EventConditionFactory::Create("OnStart")),
    _conditionB(EventConditionFactory::Create("OnStart")) {
}

editor::resources::events::OrCondition::~OrCondition() {
    delete _conditionA;
    delete _conditionB;
}

bool editor::resources::events::OrCondition::read(sol::table const& params) {
    sol::optional<sol::table> conditionA = params.get<sol::optional<sol::table>>(conditionAKey);
    if (!conditionA.has_value())
        return false;
    delete _conditionA;
    _conditionA = EventConditionFactory::Create(conditionA.value());
    if (_conditionA == nullptr)
        return false;

    sol::optional<sol::table> conditionB = params.get<sol::optional<sol::table>>(conditionBKey);
    if (!conditionB.has_value())
        return false;
    delete _conditionB;
    _conditionB = EventConditionFactory::Create(conditionB.value());
    if (_conditionB == nullptr)
        return false;

    return true;
}

bool editor::resources::events::OrCondition::render() {
    ImGui::BeginChild((std::string("##orCondition") + std::to_string(reinterpret_cast<long long>(this))).c_str(), ImVec2(0, 0), true);
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition.split.a").c_str());
    bool edited = renderConditionSelector(_conditionA);
    edited != _conditionA->render();
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition.split.b").c_str());
    edited |= renderConditionSelector(_conditionB);
    edited |= _conditionB->render();
    ImGui::EndChild();
    return edited;
}

bool editor::resources::events::OrCondition::writeParamsToEngine(sol::table& params) {
    auto& l = io::LuaManager::GetInstance().getState();
    sol::table conditionA = l.create_table();
    if (!_conditionA->writeToEngine(conditionA))
        return false;
    params[conditionAKey] = conditionA;

    sol::table conditionB = l.create_table();
    if (!_conditionB->writeToEngine(conditionB))
        return false;
    params[conditionBKey] = conditionB;

    return true;
}

bool editor::resources::events::OrCondition::writeParams(sol::table& params) {
    auto& l = io::LuaManager::GetInstance().getState();
    sol::table conditionA = l.create_table();
    if (!_conditionA->write(conditionA))
        return false;
    params[conditionAKey] = conditionA;

    sol::table conditionB = l.create_table();
    if (!_conditionB->write(conditionB))
        return false;
    params[conditionBKey] = conditionB;

    return true;
}

bool editor::resources::events::OrCondition::renderConditionSelector(EventCondition*& condition) {
    bool edited = false;
    std::string conditionID(condition->getID());
    ImGui::SetNextItemWidth(render::RenderManager::GetInstance().getWidth() / 5 - 20);
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
