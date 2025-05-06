//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "ChoicesBehaviour.h"

#include <imgui.h>
#include <io/LocalizationManager.h>

#define variableKey "variable"
#define optionsKey "options"
#define optionKeyKey "key"
#define optionValueKey "value"

#define VARIABLE_NAME_MAX_SIZE 128
#define MAX_OPTION_VALUE_SIZE 32
#define MAX_OPTION_TEXT_SIZE 32

editor::resources::events::ChoicesBehaviour::ChoicesBehaviour(Event* event) :
    EventBehaviourTemplate(event),
    _variable(new char[VARIABLE_NAME_MAX_SIZE]),
    _options() {
    _variable[0] = '\0';
}

editor::resources::events::ChoicesBehaviour::~ChoicesBehaviour() {
    delete[] _variable;
    for (auto& [text, value] : _options) {
        delete[] text;
        delete[] value;
    }
}

bool editor::resources::events::ChoicesBehaviour::read(sol::table const& params) {
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

bool editor::resources::events::ChoicesBehaviour::writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies, Object const* container) {
    behaviour << "\"" << _variable << "\", ";
    auto& luaManager = io::LuaManager::GetInstance();
    sol::table optionsValues = luaManager.getState().create_table();
    sol::table optionsTexts = luaManager.getState().create_table();
    for (auto& [optionText, optionValue] : _options) {
        optionsValues.add(optionValue);
        optionsTexts.add(optionText);
    }
    behaviour << luaManager.serializeToString(optionsValues) << ", ";
    behaviour << luaManager.serializeToString(optionsTexts);
    return true;
}

bool editor::resources::events::ChoicesBehaviour::render() {
    bool edited = renderVariableToModify();
    edited = renderOptions() || edited;
    edited = renderAddOptionButton() || edited;
    return edited;
}

bool editor::resources::events::ChoicesBehaviour::writeParams(sol::table& params) {
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


bool editor::resources::events::ChoicesBehaviour::renderVariableToModify() {
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ChoicesBehaviour.variable").c_str());
    return ImGui::InputText(("##choiceVariable" + std::to_string(reinterpret_cast<long long>(this))).c_str(), _variable, VARIABLE_NAME_MAX_SIZE);
}

bool editor::resources::events::ChoicesBehaviour::renderOptions() {
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ChoicesBehaviour.options").c_str());
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

bool editor::resources::events::ChoicesBehaviour::renderOptionValue(optionValue value, int i) {
    bool edited = ImGui::InputText(("##choiceValue" + std::to_string(i) + std::to_string(reinterpret_cast<long long>(this))).c_str(), value, MAX_OPTION_VALUE_SIZE);
    ImGui::SameLine();
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ChoicesBehaviour.value").c_str());
    return edited;
}

bool editor::resources::events::ChoicesBehaviour::renderOptionText(optionText text, int i) {
    bool edited = ImGui::InputTextMultiline(("##choiceText" + std::to_string(i) + std::to_string(reinterpret_cast<long long>(this))).c_str(), text, MAX_OPTION_TEXT_SIZE);
    ImGui::SameLine();
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ChoicesBehaviour.text").c_str());
    return edited;
}

bool editor::resources::events::ChoicesBehaviour::renderRemoveOptionButton(std::list<optionsIterator>& optionsToRemove, optionsIterator it, int i) {
    if (ImGui::Button((io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ChoicesBehaviour.remove") + "##" + std::to_string(i) + std::to_string(reinterpret_cast<long long>(this))).c_str(), ImVec2(0, 0))) {
        optionsToRemove.push_back(it);
        return true;
    }
    return false;
}

bool editor::resources::events::ChoicesBehaviour::renderAddOptionButton() {
    if (_options.size() >= 3)
        return false;
    if (ImGui::Button((io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.ChoicesBehaviour.add") + "##" + std::to_string(reinterpret_cast<long long>(this))).c_str(), ImVec2(0, 0))) {
        char* text = new char[MAX_OPTION_TEXT_SIZE];
        char* value = new char[MAX_OPTION_VALUE_SIZE];
        text[0] = '\0';
        value[0] = '\0';
        _options.emplace_back(text, value);
        return true;
    }
    return false;
}
