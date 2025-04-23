//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "ChoiceBehaviour.h"

#include <imgui.h>
#include <io/LocalizationManager.h>

#define variableKey "variable"
#define optionsKey "options"
#define optionKeyKey "key"
#define optionValueKey "value"

#define VARIABLE_NAME_MAX_SIZE 128
#define MAX_OPTION_VALUE_SIZE 32
#define MAX_OPTION_TEXT_SIZE 32

editor::resources::events::ChoiceBehaviour::ChoiceBehaviour(Event* event) :
    EventBehaviourTemplate(event),
    _variable(new char[VARIABLE_NAME_MAX_SIZE]),
    _options() {
    _variable[0] = '\0';
}

editor::resources::events::ChoiceBehaviour::~ChoiceBehaviour() {
    delete[] _variable;
    for (auto& [text, value] : _options) {
        delete[] text;
        delete[] value;
    }
}

bool editor::resources::events::ChoiceBehaviour::read(sol::table const& params) {
    sol::optional <std::string> variable = params.get<sol::optional<std::string>>(variableKey);
    if (!variable.has_value())
        return false;
    variable.value().copy(_variable, VARIABLE_NAME_MAX_SIZE);
    sol::optional<sol::table> options = params.get<sol::optional<sol::table>>(optionsKey);
    if (!options.has_value())
        return false;
    for (auto& [optionKey, optionTable] : options.value()) {
        if (!optionTable.is<sol::table>())
            return false;
        sol::optional<std::string> key = optionTable.as<sol::table>().get<sol::optional<std::string>>(optionKeyKey);
        if (!key.has_value())
            return false;
        sol::optional<std::string> value = optionTable.as<sol::table>().get<sol::optional<std::string>>(optionValueKey);
        if (!value.has_value())
            return false;
        char* text = new char[MAX_OPTION_TEXT_SIZE];
        char* optionValue = new char[MAX_OPTION_VALUE_SIZE];
        key.value().copy(text, MAX_OPTION_TEXT_SIZE);
        text[key.value().size()] = '\0';
        value.value().copy(optionValue, MAX_OPTION_VALUE_SIZE);
        optionValue[value.value().size()] = '\0';
        _options.emplace_back(text, optionValue);
    }
    return true;
}

bool editor::resources::events::ChoiceBehaviour::writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) {
    return true;
}

bool editor::resources::events::ChoiceBehaviour::render() {
    bool edited = renderVariableToModify();
    edited = renderOptions() || edited;
    edited = renderAddOptionButton() || edited;
    return edited;
}

bool editor::resources::events::ChoiceBehaviour::writeParams(sol::table& params) {
    params[variableKey] = _variable;
    auto& l = io::LuaManager::GetInstance().getState();
    sol::table options = l.create_table();
    for (auto& [key, value] : _options) {
        sol::table option = l.create_table();
        option[optionKeyKey] = key;
        option[optionValueKey] = value;
        options.add(option);
    }
    params[optionsKey] = options;
    return true;
}


bool editor::resources::events::ChoiceBehaviour::renderVariableToModify() {
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ChoiceBehaviour.variable").c_str());
    return ImGui::InputText(("##choiceVariable" + std::to_string(reinterpret_cast<long long>(this))).c_str(), _variable, VARIABLE_NAME_MAX_SIZE);
}

bool editor::resources::events::ChoiceBehaviour::renderOptions() {
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ChoiceBehaviour.options").c_str());
    if (_options.empty())
        return false;
    ImGui::BeginChild(("##choiceOptions" + std::to_string(reinterpret_cast<long long>(this))).c_str(), ImVec2(0, 0), true | ImGuiChildFlags_AutoResizeY);
    bool edited = false;
    int i = 0;
    std::list<optionsIterator> optionsToRemove;
    auto it = _options.begin();
    for (auto& [text, value] : _options) {
        ImGui::BeginChild(("##choiceOptionModify" + std::to_string(i) + std::to_string(reinterpret_cast<long long>(this))).c_str(), ImVec2(500, 0), ImGuiChildFlags_AutoResizeY);
        edited = renderOptionValue(value, i) || edited;
        edited = renderOptionText(text, i) || edited;
        ImGui::EndChild();
        ImGui::SameLine();
        edited = renderRemoveOptionButton(optionsToRemove, it, i) || edited;
        ++i;
        ++it;
        if (i < _options.size())
            ImGui::Separator();
    }
    for (auto& it : optionsToRemove) {
        _options.erase(it);
    }
    ImGui::EndChild();
    return edited;
}

bool editor::resources::events::ChoiceBehaviour::renderOptionValue(optionValue value, int i) {
    bool edited = ImGui::InputText(("##choiceValue" + std::to_string(i) + std::to_string(reinterpret_cast<long long>(this))).c_str(), value, MAX_OPTION_VALUE_SIZE);
    ImGui::SameLine();
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ChoiceBehaviour.value").c_str());
    return edited;
}

bool editor::resources::events::ChoiceBehaviour::renderOptionText(optionText text, int i) {
    bool edited = ImGui::InputTextMultiline(("##choiceText" + std::to_string(i) + std::to_string(reinterpret_cast<long long>(this))).c_str(), text, MAX_OPTION_TEXT_SIZE);
    ImGui::SameLine();
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ChoiceBehaviour.text").c_str());
    return edited;
}

bool editor::resources::events::ChoiceBehaviour::renderRemoveOptionButton(std::list<optionsIterator>& optionsToRemove, optionsIterator it, int i) {
    if (ImGui::Button((io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ChoiceBehaviour.remove") + "##" + std::to_string(i) + std::to_string(reinterpret_cast<long long>(this))).c_str(), ImVec2(0, 0))) {
        optionsToRemove.push_back(it);
        return true;
    }
    return false;
}

bool editor::resources::events::ChoiceBehaviour::renderAddOptionButton() {
    if (ImGui::Button((io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ChoiceBehaviour.add") + "##" + std::to_string(reinterpret_cast<long long>(this))).c_str(), ImVec2(0, 0))) {
        char* text = new char[MAX_OPTION_TEXT_SIZE];
        char* value = new char[MAX_OPTION_VALUE_SIZE];
        text[0] = '\0';
        value[0] = '\0';
        _options.emplace_back(text, value);
        return true;
    }
    return false;
}
