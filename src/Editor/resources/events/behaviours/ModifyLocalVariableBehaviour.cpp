//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "ModifyLocalVariableBehaviour.h"

#include <imgui.h>
#include <io/LocalizationManager.h>

#define variableKey "variable"
#define newValueKey "newValue"

#define VARIABLE_MAX_SIZE 128
#define VALUE_MAX_SIZE 32

editor::resources::events::ModifyLocalVariableBehaviour::ModifyLocalVariableBehaviour(Event* event) :
    EventBehaviourTemplate(event),
    _variable(new char[VARIABLE_MAX_SIZE]),
    _newValue(new char[VALUE_MAX_SIZE]) {
    _variable[0] = '\0';
    _newValue[0] = '\0';
}

editor::resources::events::ModifyLocalVariableBehaviour::~ModifyLocalVariableBehaviour() {
    delete[] _variable;
    delete[] _newValue;
}

bool editor::resources::events::ModifyLocalVariableBehaviour::read(sol::table const& params) {
    sol::optional<std::string> variable = params.get<sol::optional<std::string>>(variableKey);
    if (!variable.has_value())
        return false;
    variable.value().copy(_variable, VARIABLE_MAX_SIZE);
    _variable[variable.value().size()] = '\0';
    sol::optional<std::string> newValue = params.get<sol::optional<std::string>>(newValueKey);
    if (!newValue.has_value())
        return false;
    newValue.value().copy(_newValue, VALUE_MAX_SIZE);
    _newValue[newValue.value().size()] = '\0';
    return true;
}

bool editor::resources::events::ModifyLocalVariableBehaviour::writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies) {
    return true;
}

bool editor::resources::events::ModifyLocalVariableBehaviour::render() {
    bool edited = renderVariable();
    edited = renderValue() || edited;
    return edited;
}

bool editor::resources::events::ModifyLocalVariableBehaviour::writeParams(sol::table& params) {
    params[variableKey] = _variable;
    params[newValueKey] = _newValue;
    return true;
}

bool editor::resources::events::ModifyLocalVariableBehaviour::renderVariable() {
    return ImGui::InputText((io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ModifyLocalVariableBehaviour.variable") + "##" + std::to_string(reinterpret_cast<long long>(this))).c_str(), _variable, VARIABLE_MAX_SIZE);
}

bool editor::resources::events::ModifyLocalVariableBehaviour::renderValue() {
    return ImGui::InputText((io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ModifyLocalVariableBehaviour.newValue") + "##" + std::to_string(reinterpret_cast<long long>(this))).c_str(), _newValue, VALUE_MAX_SIZE);
}

