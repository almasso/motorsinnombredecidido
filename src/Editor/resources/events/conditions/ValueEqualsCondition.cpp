//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "ValueEqualsCondition.h"

#include <imgui.h>
#include <io/LocalizationManager.h>

#define variableKey "variable"
#define equalsKey "equals"

#define VARIABLE_MAX_SIZE 128
#define VALUE_MAX_SIZE 32

editor::resources::events::ValueEqualsCondition::ValueEqualsCondition() :
    _variable(new char[VARIABLE_MAX_SIZE]),
    _equals(new char[VALUE_MAX_SIZE]) {
    _variable[0] = '\0';
    _equals[0] = '\0';
}

editor::resources::events::ValueEqualsCondition::~ValueEqualsCondition() {
    delete[] _variable;
    delete[] _equals;
}

bool editor::resources::events::ValueEqualsCondition::read(sol::table const& params) {
    sol::optional<std::string> variable = params.get<sol::optional<std::string>>(variableKey);
    if (!variable.has_value())
        return false;
    variable.value().copy(_variable, VARIABLE_MAX_SIZE);
    _variable[variable.value().size()] = '\0';
    sol::optional<std::string> newValue = params.get<sol::optional<std::string>>(equalsKey);
    if (!newValue.has_value())
        return false;
    newValue.value().copy(_equals, VALUE_MAX_SIZE);
    _equals[newValue.value().size()] = '\0';
    return true;
}

bool editor::resources::events::ValueEqualsCondition::render() {
    bool edited = renderVariable();
    edited = renderValue() || edited;
    return edited;
}

bool editor::resources::events::ValueEqualsCondition::writeParamsToEngine(sol::table& params) {
    params[variableKey] = _variable;
    params[equalsKey] = _equals;
    return true;
}

bool editor::resources::events::ValueEqualsCondition::writeParams(sol::table& params) {
    params[variableKey] = _variable;
    params[equalsKey] = _equals;
    return true;
}

bool editor::resources::events::ValueEqualsCondition::renderVariable() {
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition.ValueEquals.variable").c_str());
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    return ImGui::InputText(("##" + std::to_string(reinterpret_cast<long long>(this)) + "variable").c_str(), _variable, VARIABLE_MAX_SIZE);
}

bool editor::resources::events::ValueEqualsCondition::renderValue() {
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition.ValueEquals.equals").c_str());
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    return ImGui::InputText(("##" + std::to_string(reinterpret_cast<long long>(this)) + "equals").c_str(), _equals, VALUE_MAX_SIZE);
}
