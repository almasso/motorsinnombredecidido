//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "ModifyVariableBehaviour.h"

#include <imgui.h>
#include <common/Project.h>
#include <io/LocalizationManager.h>
#include <resources/events/Event.h>

#define variableKey "variable"
#define newValueKey "newValue"
#define isPlayerVariableKey "isPlayerVariable"
#define playerVariableKey "playerVariable"

#define VARIABLE_MAX_SIZE 128
#define VALUE_MAX_SIZE 32

editor::resources::events::ModifyVariableBehaviour::ModifyVariableBehaviour(Event* event) :
    EventBehaviourTemplate(event),
    _isPlayerVariable(false),
    _variable(new char[VARIABLE_MAX_SIZE]),
    _newValue(new char[VALUE_MAX_SIZE]),
    _playerVariable("") {
    _variable[0] = '\0';
    _newValue[0] = '\0';
}

editor::resources::events::ModifyVariableBehaviour::~ModifyVariableBehaviour() {
    delete[] _variable;
    delete[] _newValue;
}

bool editor::resources::events::ModifyVariableBehaviour::read(sol::table const& params) {
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
    sol::optional<std::string> playerVariable = params.get<sol::optional<std::string>>(playerVariableKey);
    if (!playerVariable.has_value())
        return false;
    _playerVariable = playerVariable.value();
    sol::optional<bool> isPlayerVariable = params.get<sol::optional<bool>>(isPlayerVariableKey);
    if (!isPlayerVariable.has_value())
        return false;
    _isPlayerVariable = isPlayerVariable.value();
    return true;
}

bool editor::resources::events::ModifyVariableBehaviour::writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies, Object const* container) {
    if (_isPlayerVariable)
        behaviour << "\"" << _playerVariable << "\", \"" << _newValue << "\", true";
    else
        behaviour << "\"" << _variable << "\", \"" << _newValue << "\", false";
    return true;
}

bool editor::resources::events::ModifyVariableBehaviour::render() {
    bool edited = renderIsPlayerVariable();
    if (_isPlayerVariable)
        edited = renderPlayerVariable() || edited;
    else
        edited = renderVariable() || edited;
    edited = renderValue() || edited;
    return edited;
}

bool editor::resources::events::ModifyVariableBehaviour::writeParams(sol::table& params) {
    params[variableKey] = _variable;
    params[newValueKey] = _newValue;
    params[playerVariableKey] = _playerVariable;
    params[isPlayerVariableKey] = _isPlayerVariable;
    return true;
}

bool editor::resources::events::ModifyVariableBehaviour::renderIsPlayerVariable() {
    return ImGui::Checkbox((io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ModifyVariableBehaviour.isPlayerVariable") + "##isPlayerVariable" + std::to_string(reinterpret_cast<long long>(this))).c_str(), &_isPlayerVariable);
}

bool editor::resources::events::ModifyVariableBehaviour::renderPlayerVariable() {
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 2.0f);
    auto const& variables = _event->getProject()->getPlayerLocalVariables();
    std::string preview;
    if (variables->contains(_playerVariable))
        preview = _playerVariable;
    else preview = io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ModifyVariableBehaviour.selectPlayerVariable");

    if (!ImGui::BeginCombo((io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ModifyVariableBehaviour.playerVariable") + "##playerVariableSelector" + std::to_string(reinterpret_cast<long long>(this))).c_str(), preview.c_str()))
        return false;

    bool edited = false;
    for (auto& [variableName, variableValue] : *variables) {
        bool isSelected = _playerVariable == variableName;
        if(ImGui::Selectable(variableName.c_str(), isSelected)) {
            _playerVariable = variableName;
            edited = true;
        }
    }

    ImGui::EndCombo();
    return edited;
}

bool editor::resources::events::ModifyVariableBehaviour::renderVariable() {
    return ImGui::InputText((io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ModifyVariableBehaviour.variable") + "##" + std::to_string(reinterpret_cast<long long>(this))).c_str(), _variable, VARIABLE_MAX_SIZE);
}

bool editor::resources::events::ModifyVariableBehaviour::renderValue() {
    return ImGui::InputText((io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ModifyVariableBehaviour.newValue") + "##" + std::to_string(reinterpret_cast<long long>(this))).c_str(), _newValue, VALUE_MAX_SIZE);
}

