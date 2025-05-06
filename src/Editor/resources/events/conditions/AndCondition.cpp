//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "AndCondition.h"

#include <imgui.h>
#include <io/LocalizationManager.h>
#include <render/RenderManager.h>

#include "../EventConditionFactory.h"

#define conditionAKey "conditionA"
#define conditionBKey "conditionB"

editor::resources::events::AndCondition::AndCondition(Event* event) :
    EventConditionTemplate(event),
    _conditionA(EventConditionFactory::Create("OnStart", event)),
    _conditionB(EventConditionFactory::Create("OnStart", event)) {
}

editor::resources::events::AndCondition::~AndCondition() {
    delete _conditionA;
    delete _conditionB;
}

bool editor::resources::events::AndCondition::read(sol::table const& params) {
    sol::optional<sol::table> conditionA = params.get<sol::optional<sol::table>>(conditionAKey);
    if (!conditionA.has_value())
        return false;
    delete _conditionA;
    _conditionA = EventConditionFactory::Create(conditionA.value(), _event);
    if (_conditionA == nullptr)
        return false;

    sol::optional<sol::table> conditionB = params.get<sol::optional<sol::table>>(conditionBKey);
    if (!conditionB.has_value())
        return false;
    delete _conditionB;
    _conditionB = EventConditionFactory::Create(conditionB.value(), _event);
    if (_conditionB == nullptr)
        return false;

    return true;
}

bool editor::resources::events::AndCondition::render() {
    ImGui::BeginChild((std::string("##andCondition") + std::to_string(reinterpret_cast<long long>(this))).c_str(), ImVec2(0, 0), true | ImGuiChildFlags_AutoResizeY);
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition.split.a").c_str());
    bool edited = renderConditionSelector(_conditionA);
    edited = _conditionA->render() || edited;
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition.split.b").c_str());
    edited = renderConditionSelector(_conditionB) || edited;
    edited = _conditionB->render() || edited;
    ImGui::EndChild();
    return edited;
}

bool editor::resources::events::AndCondition::writeParamsToEngine(std::ostream& condition, EventBuildDependencies& dependencies, Object const* container) {
    condition << conditionAKey << " = {\n";
    if (!_conditionA->writeToEngine(condition, dependencies, container))
        return false;
    condition << "}\n";

    condition << conditionBKey << " = {\n";
    if (!_conditionB->writeToEngine(condition, dependencies, container))
        return false;
    condition << "}\n";

    return true;
}

bool editor::resources::events::AndCondition::writeParams(sol::table& params) {
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

bool editor::resources::events::AndCondition::renderConditionSelector(EventCondition*& condition) {
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
                condition = EventConditionFactory::Create(conditionName, _event);
                edited = true;
            }
        }
    }
    ImGui::EndCombo();
    return edited;
}